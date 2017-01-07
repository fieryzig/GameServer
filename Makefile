


clean:
	find ./ -name *.o | xargs rm -f
	find ./ -name *.cpp~ | xargs rm -f
	find ./ -name *.h~ | xargs rm -f
	find ./ -name *.json~ | xargs rm -f
	find ./ -name *.un~ | xargs rm -f
	rm -rf Makefile~
	
