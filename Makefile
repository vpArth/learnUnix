EXECUTABLES=ls cat info sh err sigs files fd

.PHONY: clean add
clean:
	rm -f $(EXECUTABLES)
add: clean
	git add --all
	git status

