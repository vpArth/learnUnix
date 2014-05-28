EXECUTABLES=ls cat info

.PHONY: clean add
clean:
	rm -f $(EXECUTABLES)
add: clean
	git add --all
	git status

