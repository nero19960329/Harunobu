#include <harunobu/rfilter/box_filter.h>
#include <harunobu/rfilter/rfilter_base.h>
#include <harunobu/rfilter/tent_filter.h>

HARUNOBU_NAMESPACE_BEGIN

sptr<RFilterBase> RFilterBase::factory(std::string name) {
    if (name == "box") {
        sptr<BoxFilter> rfilter = std::make_shared<BoxFilter>();
        return rfilter;
    } else if (name == "tent") {
        sptr<TentFilter> rfilter = std::make_shared<TentFilter>();
        return rfilter;
    } else {
        HARUNOBU_CHECK(false, "Unsupported rfilter {}.", name);
    }
}

HARUNOBU_NAMESPACE_END