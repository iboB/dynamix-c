#include <dnmx/mixin_type_info.h>
#include <dnmx/domain.h>

int main() {
    dnmx_mixin_type_info info = {0};

    dnmx_domain dom = {0};

    info.name = "gosho";
    info.size = 8;
    info.alignment = 8;

    dnmx_domain_register_mixin(&dom, &info);

    dnmx_domain_clear(&dom);

    return 0;
}