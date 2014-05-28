EXECUTABLES=ls cat info sh

.PHONY: clean add
clean:
	rm -f $(EXECUTABLES)
add: clean
	git add --all
	git status

