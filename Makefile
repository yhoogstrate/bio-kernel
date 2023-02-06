
all: db/taxons/taxons

db/taxons/taxons:
	cd db/taxons; virtualenv -p python3 .venv; source .venv/bin/activate ; pip3 install pywget tqdm; python3 ./update-taxons.py

