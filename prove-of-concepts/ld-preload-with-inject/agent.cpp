#include <frida-gum.h>
#include <algorithm>
#include <iostream>
#include <ostream>
#include <string>
#include <dlfcn.h>

using puts_fn = int (*)(const char *);

static puts_fn orig_puts_fn = nullptr;

extern "C" int new_puts(const char *str)
{
	std::string s(str);
	std::reverse(s.begin(), s.end());
	s = "<Reversed puts: " + s + ">";
	std::cout << s << std::endl;
	return 0;
}

extern "C" int puts(const char *str)
{
	if (orig_puts_fn) {
		orig_puts_fn = (puts_fn)dlsym(RTLD_NEXT, "puts");
	}
	return new_puts(str);
}

extern "C" void agent_main(const gchar *data, gboolean *stay_resident)
{
	gum_init_embedded();

	*stay_resident = TRUE;
	auto interceptor = gum_interceptor_obtain();
	gum_interceptor_begin_transaction(interceptor);
	auto puts_fn = gum_module_find_export_by_name(nullptr, "puts");
	void *orig_fn;
	gum_interceptor_replace(interceptor, reinterpret_cast<void *>(puts_fn),
				reinterpret_cast<void *>(new_puts), nullptr,
				&orig_fn);

	gum_interceptor_end_transaction(interceptor);
}
