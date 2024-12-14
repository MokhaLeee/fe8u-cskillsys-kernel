TMX2EA requires python 3.

[`requirements.txt`](./requirements.txt) is provided so you can simply do the following:

	pip install -r requirements.txt

From that, you should be able to run tmx2ea through `python tmx2ea.py`, and even generate a releasable build through `make`.

JESTER - If you get an error about `No module named 'tkinter'`, then run `sudo apt-get install python3-tk` to install it