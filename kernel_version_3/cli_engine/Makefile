all:
	@echo "Only cscope of python code is allowed"
	@echo "Use make cs to start cscope for python code"

cs:
	ctags -R
	find . -name '*.py' > cscope.files && cscope -Rb
	cscope -d
