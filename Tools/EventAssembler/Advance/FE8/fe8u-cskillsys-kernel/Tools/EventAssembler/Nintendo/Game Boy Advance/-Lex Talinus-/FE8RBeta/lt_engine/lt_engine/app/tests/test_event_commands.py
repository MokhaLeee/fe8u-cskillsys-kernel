import unittest
import logging

from app.events import event_commands
from app.events.event_structs import EOL

class EventCommandUnitTests(unittest.TestCase):
    def setUp(self):
        # Patch out logging below CRITICAL DAMAGE
        logging.disable(logging.CRITICAL)

    def tearDown(self):
        # re-enable logging
        logging.disable(logging.NOTSET)

    def test_subclasses(self):
        # Make sure there are some commands
        commands = event_commands.get_commands()
        self.assertTrue(len(commands) > 0)
        # Make sure that speak exists as a command
        self.assertTrue(any(command.nid == 'speak' for command in commands))

    def test_catalog(self):
        from app.events import function_catalog
        catalog = function_catalog.get_catalog()
        self.assertTrue(len(catalog) > 0)

    def test_validators(self):
        from app.events import event_validators
        commands = event_commands.get_commands()
        invalid_keywords = set()
        for command in commands:
            keywords = command.get_keyword_types()
            for keyword in keywords:
                if(event_validators.get(keyword) is None):
                    invalid_keywords.add((keyword, command.nid))
        self.assertEqual(0, len(invalid_keywords), "Invalid keywords found: " + ', '.join(map(str, invalid_keywords)))

    def test_determine_command_type(self):
        command1 = event_commands.determine_command_type("speak")
        self.assertTrue(command1 == event_commands.Speak)
        # Test with tab in front
        command2 = event_commands.determine_command_type("    speak;Eirika;I am evil;;60")
        self.assertTrue(command2 == event_commands.Speak)

    def test_determine_command_type_nickname(self):
        command = event_commands.determine_command_type("s;Eirika;I am evil;;60")
        self.assertTrue(command == event_commands.Speak)

    def test_determine_command_type_comment(self):
        command1 = event_commands.determine_command_type("#Hello")
        self.assertTrue(command1 == event_commands.Comment)
        command2 = event_commands.determine_command_type("comment;Hello")
        self.assertTrue(command2 == event_commands.Comment)

    def test_determine_command_type_fails(self):
        command = event_commands.determine_command_type("flgjklskl;sumerian;lorem ipsum")
        self.assertTrue(command == event_commands.Comment)

    def test_parse_text_to_command_typing(self):
        # Missing keyword
        command1, bad_idx = event_commands.parse_text_to_command("speak")
        self.assertTrue(isinstance(command1, event_commands.Comment))
        # Test with tab in front
        command2, bad_idx = event_commands.parse_text_to_command("    speak;Eirika;I am evil;;60")
        self.assertTrue(isinstance(command2, event_commands.Speak))
        # Test with nickname
        command3, bad_idx = event_commands.parse_text_to_command("s;Eirika;I am evil")
        self.assertTrue(isinstance(command3, event_commands.Speak))
        # Test comments
        command4, bad_idx = event_commands.parse_text_to_command("#Hello")
        self.assertTrue(isinstance(command4, event_commands.Comment))
        command5, bad_idx = event_commands.parse_text_to_command("comment;Hello")
        self.assertTrue(isinstance(command5, event_commands.Comment))
        command6, bad_idx = event_commands.parse_text_to_command("flgjklskl;sumerian;lorem ipsum")
        self.assertTrue(isinstance(command6, event_commands.Comment))
        command7, bad_idx = event_commands.parse_text_to_command(None)
        self.assertTrue(command7 == event_commands.Comment())
        command8, bad_idx = event_commands.parse_text_to_command("")
        self.assertTrue(command8 == event_commands.Comment())

    def test_parse_text_to_command_strict(self):
        # Test with keyword arguments
        command1, bad_idx = event_commands.parse_text_to_command("set_current_hp;Unit=Seth;HP=13", strict=True)
        self.assertTrue(isinstance(command1, event_commands.SetCurrentHP))
        self.assertEqual(command1.parameters.get('Unit'), "Seth")
        self.assertEqual(command1.parameters.get('HP'), "13")
        self.assertEqual(len(command1.chosen_flags), 0)
        self.assertTrue(bad_idx is None)
        # Test with flags and nickname
        command2, bad_idx = event_commands.parse_text_to_command("r;Seth;no_block", strict=True)
        self.assertTrue(isinstance(command2, event_commands.RemovePortrait))
        self.assertEqual(command2.parameters.get('Portrait'), "Seth")
        self.assertEqual(command2.chosen_flags, {"no_block"})
        # Test with universal flags
        command3, bad_idx = event_commands.parse_text_to_command("set_current_hp;Seth;13;no_warn", strict=True)
        self.assertEqual(command3.chosen_flags, {"no_warn"})
        # Test with display values (use of parentheses)
        command4, bad_idx = event_commands.parse_text_to_command("set_current_hp;Seth;13", strict=True)
        self.assertEqual(command4.parameters.get('Unit'), "Seth")
        self.assertEqual(command4.parameters.get('HP'), "13")
        # Test with evaluables
        command5, bad_idx = event_commands.parse_text_to_command("if;Seth (Seth)", strict=True)
        self.assertEqual(command5.parameters.get('Expression'), "Seth (Seth)")
        # Test with positional arguments before keywords
        command6, bad_idx = event_commands.parse_text_to_command("set_current_hp;Seth;HP=13", strict=True)
        self.assertEqual(command6.parameters.get('Unit'), "Seth")
        self.assertEqual(command6.parameters.get('HP'), "13")
        # Test with invalid keyword argument
        command7, bad_idx = event_commands.parse_text_to_command("set_current_hp;Unit=Seth;13", strict=True)
        self.assertTrue(command7 is None)
        self.assertEqual(bad_idx, 1)
        # Test with using keyword types wrongly
        command8, bad_idx = event_commands.parse_text_to_command("set_current_hp;Unit=Seth;PositiveInteger=13", strict=True)
        self.assertTrue(command8 is None)
        self.assertEqual(bad_idx, 1)
        # Purposefully does not do validation of keyword arguments here,
        # so that other functions can use the full command when THEY do validation
        # Plus, validation often reuqires that the validator know what level is referred to
        # This ruins the atomic self-containedness of theis
        # parse_text_to_command is really a structural check, not a semantic check
        command9, bad_idx = event_commands.parse_text_to_command("set_current_hp;Unit=Seth;HP=lorem ipsum", strict=True)
        self.assertTrue(isinstance(command9, event_commands.SetCurrentHP))
        # Test with too many arguments
        command10, bad_idx = event_commands.parse_text_to_command("set_current_hp;Seth;13;Lorem Ipsum", strict=True)
        self.assertTrue(command10 is None)
        self.assertEqual(bad_idx, 2)
        # Test with non-existent keyword argument
        command11, bad_idx = event_commands.parse_text_to_command("set_current_hp;Seth;13;Lorem=Ipsum", strict=True)
        self.assertTrue(command11 is None)
        self.assertEqual(bad_idx, 2)
        # Test with missing keyword arguments
        command12, bad_idx = event_commands.parse_text_to_command("set_current_hp;Seth", strict=True)
        self.assertTrue(command12 is None)
        # Test with trailing semicolon
        command13, bad_idx = event_commands.parse_text_to_command("else;", strict=True)
        self.assertTrue(isinstance(command13, event_commands.Else))
        self.assertEqual(len(command13.parameters), 0)
        # Test with flags
        command14, bad_idx = event_commands.parse_text_to_command("give_item;Eirika;Rapier;no_banner", strict=True)
        self.assertTrue(isinstance(command14, event_commands.GiveItem))
        self.assertEqual(len(command14.chosen_flags), 1)
        self.assertTrue('no_banner' in command14.chosen_flags)

    def test_parse(self):
        input_parameters = {"x": 1, "y": 2}
        x = event_commands.EventCommand(input_parameters, {"algebra"})
        parameters, flags = event_commands.parse(x)
        self.assertEqual(parameters, input_parameters)
        self.assertEqual(flags, {"algebra"})

    def test_convert_parse(self):
        input_parameters = {"x": 1, "y": 2}
        x = event_commands.EventCommand(input_parameters, {"algebra"})
        parameters, flags = event_commands.convert_parse(x)
        self.assertEqual(parameters, input_parameters)
        self.assertEqual(flags, {"algebra"})
        # Test with existing command and make sure it returns back parameters + flags
        command_parameters = {"Unit": "Seth", "HP": "13"}
        command1 = event_commands.SetCurrentHP(command_parameters, {"no_warn"})
        parameters, flags = event_commands.convert_parse(command1)
        self.assertEqual(parameters, command_parameters)
        self.assertEqual(flags, {"no_warn"})
        # Test with existing command and see if it runs convert using validators
        command_parameters = {"Speaker": "Seth", "Speed": "5.5"}
        command2 = event_commands.SpeakStyle(command_parameters)
        parameters, flags = event_commands.convert_parse(command2)
        self.assertEqual(parameters, {"Speaker": "Seth", "Speed": 5.5})
        self.assertTrue(len(flags) == 0)
        # Test with flags and exising function
        command3, _ = event_commands.parse_text_to_command("give_item;Eirika;Rapier;no_banner", strict=True)
        self.assertTrue(isinstance(command3, event_commands.GiveItem))
        self.assertEqual(len(command3.chosen_flags), 1)
        self.assertEqual(command3.chosen_flags, {"no_banner"})
        parameters, flags = event_commands.convert_parse(command3)
        self.assertEqual(flags, {"no_banner"})

    def test_save_and_restore(self):
        # Test save returns what we expect
        command, _ = event_commands.parse_text_to_command("set_current_hp;Unit=Seth;HP=13")
        dat = command.save()
        self.assertEqual(len(dat), 2)
        self.assertEqual(dat[0], command.nid)
        # Test restore correctly restores an event command
        command = event_commands.restore_command(dat)
        self.assertTrue(isinstance(command, event_commands.SetCurrentHP))
        parameters = {"Unit": "Seth", "HP": "13"}
        self.assertEqual(command.parameters, parameters)

    def test_save_and_restore_comment(self):
        command = event_commands.Comment(display_values=["Hello"])
        dat = command.save()
        self.assertEqual(dat[0], command.nid)
        self.assertEqual(dat[1], ["Hello"])
        command = event_commands.restore_command(dat)
        self.assertEqual(command.display_values, ["Hello"])

    def test_get_keyword_types(self):
        # Test that when keyword types exist, it returns that
        command = event_commands.SetCurrentHP()
        self.assertEqual(command.get_keyword_types(), command.keyword_types)
        # Test that when keyword types does not exist, it returns regular command
        command = event_commands.If()
        self.assertEqual(command.get_keyword_types(), command.keywords + command.optional_keywords)
        command = event_commands.Sound()
        self.assertEqual(command.get_keyword_types(), command.keywords + command.optional_keywords)

    def test_get_validator_from_keyword(self):
        command = event_commands.Music()
        # Test validator from keywords
        self.assertEqual(command.get_validator_from_keyword("Music"), "Music")
        # Test validator from optional keywords
        self.assertEqual(command.get_validator_from_keyword("FadeIn"), "Time")

    def test_get_index_from_keyword(self):
        command, _ = event_commands.parse_text_to_command("add_portrait;ScreenPosition=Left;Slide=Left;Portrait=Eirika")
        self.assertTrue(isinstance(command, event_commands.AddPortrait))
        self.assertEqual(len(command.display_values), 3)
        self.assertTrue("Portrait=Eirika" in command.display_values)
        # Test keyword argument
        self.assertEqual(command.get_index_from_keyword("ScreenPosition"), 0)
        # Test positional argument
        self.assertEqual(command.get_index_from_keyword("Slide"), 1)
        self.assertEqual(command.get_index_from_keyword("Portrait"), 2)

    def test_check_event_signatures(self):
        from app.utilities import str_utils

        commands = event_commands.get_commands()
        for command in commands:
            # Make sure no duplicates in command.keywords and command.optional_keywords
            all_keywords = command.keywords + command.optional_keywords
            self.assertEqual(len(set(all_keywords)), len(all_keywords), command.nid)
            # Make sure we have the right amount of keyword_types
            if command.keyword_types:
                self.assertEqual(len(command.keyword_types), len(all_keywords), command.nid)
            # Make sure the command's class name and nid follow conventions
            self.assertEqual(str_utils.camel_to_snake(command().__class__.__name__), command.nid)

    def test_check_event_function_flags(self):
        import inspect
        from app.events import function_catalog
        # Make sure event_commands match properly with event_functions
        # Make sure every command can ingest flags=None
        catalog = function_catalog.get_catalog()
        for func in catalog.values():
            sig = inspect.signature(func)
            self_parameter = sig.parameters.get('self')
            self.assertTrue(self_parameter is not None, func)
            flag_parameter = sig.parameters.get('flags')
            self.assertTrue(flag_parameter is not None, func)
            self.assertTrue(flag_parameter.default is None, func)
            # Make sure flags is always the last parameter
            self.assertTrue(flag_parameter is list(sig.parameters.values())[-1], func)

    def test_check_event_functions_match_event_commands(self):
        import inspect
        from app.utilities import str_utils
        from app.events import function_catalog

        commands = event_commands.get_commands()
        catalog = function_catalog.get_catalog()
        for command in commands:
            if command.special_handling:  # If, Elif, Else, End, For, Endf
                continue
            all_keywords = command.keywords + command.optional_keywords
            self.assertTrue(command.nid in catalog, command.nid)  # There is a corresponding event function
            associated_func = catalog[command.nid]
            sig = inspect.signature(associated_func)
            self.assertEqual(len(sig.parameters.keys()), len(all_keywords) + 2, '%s: %s: %s %s' % (command.nid, sig, sig.parameters, sig.parameters.keys())) # Also includes self: Event and flags=None

            for idx, keyword in enumerate(all_keywords):
                traceback_str = '%s: %s' % (command.nid, keyword)
                # remove * for array args
                snake_case = str_utils.camel_to_snake(keyword.removeprefix('*'))
                self.assertTrue(snake_case in sig.parameters, traceback_str)
                # +1 because of self in the function signature
                self.assertEqual(idx + 1, list(sig.parameters.keys()).index(snake_case), traceback_str)
                param = sig.parameters[snake_case]
                if keyword in command.optional_keywords:
                    self.assertTrue(param.default is not param.empty, traceback_str)
                else:
                    self.assertTrue(param.default is param.empty, traceback_str)

    def test_parse_event_line_to_tokens(self):
        command = 'speak;Eirika;Hello;no_block'
        toks = event_commands.parse_event_line(command)
        self.assertEqual(toks.tokens, ['speak', 'Eirika', 'Hello', 'no_block'])
        self.assertEqual(toks.source, command)
        self.assertEqual(toks.token_idx, [0, 6, 13, 19])
        self.assertEqual(toks._flag_idx, 3)

        command2 = 'speak;Eirika;Hello;no_block;no_warn'
        toks2 = event_commands.parse_event_line(command2)
        self.assertEqual(toks2.flags(), ['no_block', 'no_warn'])

        command3 = ' speak;Eirika;Hello'
        toks3 = event_commands.parse_event_line(command3)
        self.assertEqual(toks3.token_idx, [1, 7, 14])

        command4 = 'speak;Eirika;;;'
        toks4 = event_commands.parse_event_line(command4)
        self.assertEqual(toks4.tokens, ['speak', 'Eirika', '', '', ''])

        command5 = 'speak;Eirika #this is a comment'
        toks5 = event_commands.parse_event_line(command5)
        self.assertEqual(toks5.tokens, ['speak', 'Eirika ', EOL])

        command7 = 'speak;Eirika;nested {c:wait;500};NumLines=1'
        toks7 = event_commands.parse_event_line(command7)
        self.assertEqual(toks7.tokens, ['speak', 'Eirika', 'nested {c:wait;500}', 'NumLines=1'])