compile: yurifetch.cpp api.h
	g++ yurifetch.cpp -o yurifetch -lcurl -lpugixml
clean: yurifetch
	rm yurifetch
