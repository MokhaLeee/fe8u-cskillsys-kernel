from typing import List
from app.events.event_commands import EventCommand
from app.events.event_prefab import EventPrefab
from app.events.triggers import EventTrigger
from app.data.database.database import DB
from app.events.event import Event
from app.events import triggers
from app.engine.game_state import game
from app.engine import action, evaluate

import logging

class EventManager():
    def __init__(self):
        self.all_events = []  # Keeps all events, both in use and not yet used
        self.event_stack = []  # A stack of events that haven't been used yet

    def get_triggered_events(self, trigger: EventTrigger, level_nid=None):
        """returns a list of all events that are triggered according to the conditions supplied in the arg
        """
        triggered_events = []
        if level_nid:
            event_source_nid = level_nid
        else:
            if game.level_nid:
                event_source_nid = game.level_nid
            else:
                event_source_nid = None
        for event_prefab in DB.events.get(trigger.nid, event_source_nid):
            try:
                args = trigger.to_args()
                result = evaluate.evaluate(event_prefab.condition, unit1=args.get('unit1', None), unit2=args.get('unit2', None), position=args.get('position', None), local_args=args)
                if event_prefab.nid not in game.already_triggered_events and result:
                    triggered_events.append(event_prefab)
            except:
                logging.error("Condition {%s} could not be evaluated" % event_prefab.condition)
        return triggered_events

    def should_trigger(self, trigger: EventTrigger, level_nid=None):
        """Check whether or not there are any events to trigger for the conditions given
        """
        triggered_events = self.get_triggered_events(trigger, level_nid)
        return len(triggered_events) > 0

    def trigger(self, trigger: EventTrigger, level_nid=None) -> bool:
        """
        Returns whether an event was triggered!
        """
        triggered_events = self.get_triggered_events(trigger, level_nid)
        new_event = False
        sorted_events = sorted(triggered_events, key=lambda x: x.priority)
        for event_prefab in sorted_events:
            self._add_event(event_prefab, trigger)
            new_event = True
            if event_prefab.only_once:
                action.do(action.OnlyOnceEvent(event_prefab.nid))
        return new_event

    def trigger_specific_event(self, event_nid, unit=None, unit2=None, position=None, local_args=None, force=False):
        event_prefab = DB.events.get_from_nid(event_nid)

        # filter OnlyOnceEvents
        if not force:
            if event_prefab.nid in game.already_triggered_events:
                return False

        self._add_event(event_prefab, triggers.GenericTrigger(unit, unit2, position, local_args))
        if event_prefab.only_once:
            action.do(action.OnlyOnceEvent(event_prefab.nid))
        return True

    def _add_event_from_script(self, nid, script: str, trigger: EventTrigger):
        dummy_prefab = EventPrefab(nid)
        dummy_prefab.source = script
        self._add_event(dummy_prefab, trigger)

    def _add_event(self, event_prefab, trigger: EventTrigger):
        event = Event(event_prefab, trigger)
        self.all_events.append(event)
        self.event_stack.append(event)
        game.state.change('event')

    def append(self, event):
        self.all_events.append(event)
        self.event_stack.append(event)

    def get(self):
        if self.event_stack:
            return self.event_stack.pop()
        return None

    def end(self, event):
        if event in self.all_events:
            self.all_events.remove(event)

    def clear(self):
        self.event_stack.clear()
        self.all_events.clear()

    def save(self):
        all_events = [event.save() for event in self.all_events]
        return all_events

    @classmethod
    def restore(cls, all_events=None):
        self = cls()
        if all_events is None:
            all_events = []
        for event in all_events:
            new_event = Event.restore(event, game)
            self.all_events.append(new_event)
            self.event_stack.append(new_event)

        return self
