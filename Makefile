CC = gcc

all: bin/console
	cd mastermind-gui && mvn package
	mv mastermind-gui/target/mastermind-1.0-SNAPSHOT.jar bin/mastermind.jar

bin/console: mastermind-cli/main.c
	$(CC) -o bin/console -W -Wall mastermind-cli/main.c
