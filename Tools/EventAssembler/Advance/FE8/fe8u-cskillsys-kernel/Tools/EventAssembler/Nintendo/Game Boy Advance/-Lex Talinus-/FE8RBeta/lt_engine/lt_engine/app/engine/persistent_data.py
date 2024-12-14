try:
    import cPickle as pickle
except ImportError:
    import pickle
import logging

def serialize(location, data):
    logging.info("Saving persistent data to %s", location)
    with open(location, 'wb') as fp:
        try:
            pickle.dump(data, fp)
        except TypeError as e:
            # There's a surface somewhere in the dictionary of things to save...
            print(data)
            print(e)

def deserialize(location):
    logging.info("Loading persistent data from %s", location)
    try:
        with open(location, 'rb') as fp:
            s_dict = pickle.load(fp)
            return s_dict
    except FileNotFoundError:
        logging.info("No persistent data file found at %s", location)

def clear(location):
    logging.info("Clearing data in %s", location)
    with open(location, 'wb') as fp:
        pickle.dump([], fp)
