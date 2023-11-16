SHELL ?= /bin/bash

agent.so: agent.cpp hooker.so
	$(CXX) -shared -Wall -fPIC -DAGENT_SO -O2 -g $< -o $@ ./frida/libfrida-gum.a -std=c++20 -lopcodes
agent.test: agent.cpp hooker.so
	$(CXX) -O2 -Wall -g $< -o $@ ./frida/libfrida-gum.a -std=c++20 -lopcodes

hooker.so: hooker.cpp
	$(CXX) -shared -Wall -fPIC -O2 -g $< -o $@ -std=c++20
injector: injector.cpp agent.so
	$(CXX)  -g $< -o $@ -Wall ./frida/libfrida-core.a
victim: victim.cpp
	$(CXX) -O2 -g $< -Wall -o $@ 

run: injector victim
	@./victim & pid=$$!; echo "Victim pid: $$pid" && ./injector $$pid && wait $$pid
clean:
	rm -rf *.so *.o injector victim out.txt *.ii *.s *.out

frida-lock:
	rm -rf frida
	mkdir frida
	cd frida && wget https://github.com/frida/frida/releases/download/16.1.3/frida-core-devkit-16.1.3-linux-x86_64.tar.xz && tar --xz -xvf frida-core-devkit-16.1.3-linux-x86_64.tar.xz
	cd frida && wget https://github.com/frida/frida/releases/download/16.1.3/frida-gum-devkit-16.1.3-linux-x86_64.tar.xz && tar --xz -xvf frida-gum-devkit-16.1.3-linux-x86_64.tar.xz
	touch frida-lock
