#include "../cotl"

namespace cotlstd {

// arg: self, caller, lib, tunnel

_COTL_FUNC_T(stdIncrement)
_COTL_FUNC_BEGIN
    if (auto self_p = self.raw<cotl::Int>()) {
        if (tunnel) {
            if (auto tunnel_p = tunnel.as<cotl::Int>()) {
                self_p->getVar() = tunnel_p->get();
                tunnel = nullptr;
            } else {
                throw;
            }
        } else {
            ++(self_p->getVar());
        }
    } else {
        throw;
    }
_COTL_FUNC_END

_COTL_FUNC_T(stdDecrement)
_COTL_FUNC_BEGIN
    if (auto self_p = self.raw<cotl::Int>()) {
        if (tunnel) {
            if (auto tunnel_p = tunnel.as<cotl::Int>()) {
                self_p->getVar() = tunnel_p->get();
                tunnel = nullptr;
            } else {
                throw;
            }
        } else {
            --(self_p->getVar());
        }
    } else {
        throw;
    }
_COTL_FUNC_END

_COTL_FUNC_T(stdRefPtr)
_COTL_FUNC_BEGIN
    if (auto self_p = self.as<cotl::Ptr>()) {
        self_p->get()(caller, lib, tunnel); // _COTL_CALL
    } else {
        throw;
    }
_COTL_FUNC_END

_COTL_FUNC_T(stdStack)
_COTL_FUNC_BEGIN
    if (auto self_p = self.raw<cotl::Arr>()) {
        arr_t &data = self_p->getVar();
        if (tunnel) {
            data->push_back(tunnel);
            tunnel = nullptr;
        } else {
            tunnel = data->back();
            data->pop_back();
        }
    } else {
        throw;
    }
_COTL_FUNC_END

namespace {

_COTL_FUNC_T(init)
_COTL_FUNC_BEGIN
    PMaybe caller1(nullptr);
    PMaybe lib1(nullptr);
    PMaybe tunnel1(nullptr);

    caller1 = _atom(id_increment);
    tunnel1 = _func(stdIncrement, id_func, stdLibFunc);
    lib.call<false>(caller1, lib1, tunnel1); // _COTL_CALL

    caller1 = _atom(id_decrement);
    tunnel1 = _func(stdDecrement, id_func, stdLibFunc);
    lib.call<false>(caller1, lib1, tunnel1); // _COTL_CALL

    caller1 = _atom(id_refptr);
    tunnel1 = _func(stdRefPtr, id_func, stdLibFunc);
    lib.call<false>(caller1, lib1, tunnel1); // _COTL_CALL

    caller1 = _atom(id_stack);
    tunnel1 = _func(stdStack, id_func, stdLibFunc);
    lib.call<false>(caller1, lib1, tunnel1); // _COTL_CALL
_COTL_FUNC_END

long long ago = addInitializer(init);

}

}