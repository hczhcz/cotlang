#include "../cotl"

namespace cotl {

namespace stdlib {

// arg: self, caller, lib, tunnel

_COTL_FUNC_T(stdWrapAtom)
_COTL_FUNC_BEGIN
    _COTL_CHECK_SELF(Atom, id_atom);
    _COTL_CHECK_TUNNEL(false);

    tunnel = _atom(
        self_p->getType(), self_p->getFunc()
    );
_COTL_FUNC_END

_COTL_FUNC_T(stdWrapInt)
_COTL_FUNC_BEGIN
    _COTL_CHECK_SELF(Int, id_int);
    _COTL_CHECK_TUNNEL(false);

    tunnel = _int(
        self_p->get(),
        self_p->getType(), self_p->getFunc()
    );
_COTL_FUNC_END

_COTL_FUNC_T(stdWrapReal)
_COTL_FUNC_BEGIN
    _COTL_CHECK_SELF(Real, id_real);
    _COTL_CHECK_TUNNEL(false);

    tunnel = _real(
        self_p->get(),
        self_p->getType(), self_p->getFunc()
    );
_COTL_FUNC_END

_COTL_FUNC_T(stdWrapFunc)
_COTL_FUNC_BEGIN
    _COTL_CHECK_SELF(Func, id_func);
    _COTL_CHECK_TUNNEL(false);

    tunnel = _func(
        self_p->get(),
        self_p->getType(), self_p->getFunc()
    );
_COTL_FUNC_END

_COTL_FUNC_T(stdWrapPtr)
_COTL_FUNC_BEGIN
    _COTL_CHECK_SELF(Ptr, id_ptr);

    if (tunnel) {
        _COTL_CHECK_TYPE_ANY(tunnel, Ptr, id_ptr);

        PMaybe tunnel1(tunnel_p->get());

        self_p->get().call<false>(caller, lib, tunnel1);

        tunnel = nullptr;
    } else {
        PMaybe tunnel1(nullptr);

        self_p->get().call<true>(caller, lib, tunnel1);

        tunnel = _ptr(
            PVal(tunnel1),
            self_p->getType(), self_p->getFunc()
        );
    }
_COTL_FUNC_END

_COTL_FUNC_T(stdWrapPair)
_COTL_FUNC_BEGIN
    _COTL_CHECK_SELF(Pair, id_pair);

    if (tunnel) {
        _COTL_CHECK_TYPE_ANY(tunnel, Pair, id_pair);

        PMaybe tunnel1(tunnel_p->get().first);
        PMaybe tunnel2(tunnel_p->get().second);

        self_p->get().first.call<false>(caller, lib, tunnel1);
        self_p->get().second.call<false>(caller, lib, tunnel2);

        tunnel = nullptr;
    } else {
        PMaybe tunnel1(nullptr);
        PMaybe tunnel2(nullptr);

        self_p->get().first.call<true>(caller, lib, tunnel1);
        self_p->get().second.call<true>(caller, lib, tunnel2);

        tunnel = _pair(
            PVal(tunnel1), PVal(tunnel2),
            self_p->getType(), self_p->getFunc()
        );
    }
_COTL_FUNC_END

_COTL_FUNC_T(stdWrapStr)
_COTL_FUNC_BEGIN
    _COTL_CHECK_SELF(Str, id_str);
    _COTL_CHECK_TUNNEL(false);

    tunnel = _str(
        *(self_p->get()),
        self_p->getType(), self_p->getFunc()
    );
_COTL_FUNC_END

_COTL_FUNC_T(stdWrapArr)
_COTL_FUNC_BEGIN
    _COTL_CHECK_SELF(Arr, id_arr);

    if (tunnel) {
        _COTL_CHECK_TYPE_ANY(tunnel, Arr, id_arr);

        const arr_t &self_data = self_p->get();
        const arr_t &tunnel_data = tunnel_p->get();

        if (self_data->size() == tunnel_data->size()) {
            // notice: without range check
            for (size_t i = 0; i != self_data->size(); ++i) {
                PMaybe tunnel1((*tunnel_data)[i]);

                (*self_data)[i].call<false>(caller, lib, tunnel1);
            }

            tunnel = nullptr;
        } else {
            throw "bad arr size";
        }
    } else {
        auto tunnel_p = _arr(self_p->getType(), self_p->getFunc());

        const arr_t &self_data = self_p->get();
        const arr_t &tunnel_data = tunnel_p->get();

        // notice: without range check
        for (size_t i = 0; i != self_data->size(); ++i) {
            PMaybe tunnel1(nullptr);

            (*self_data)[i].call<true>(caller, lib, tunnel1);

            PVal helper(tunnel1); // for libstdc++ without gc
            tunnel_data->push_back(helper);
        }

        tunnel = tunnel_p;
    }
_COTL_FUNC_END

_COTL_FUNC_T(stdWrapMap)
_COTL_FUNC_BEGIN
    _COTL_CHECK_SELF(Map, id_map);

    if (tunnel) {
        _COTL_CHECK_TYPE_ANY(tunnel, Map, id_map);

        const map_t &self_data = self_p->get();
        const map_t &tunnel_data = tunnel_p->get();

        for (auto &val: *self_data) {
            if (tunnel_data->count(val.first)) {
                PMaybe tunnel1(tunnel_data->at(val.first));

                val.second.call<false>(caller, lib, tunnel1);
            } else {
                throw "bad map key";
            }
        }

        tunnel = nullptr;
    } else {
        auto tunnel_p = _map(self_p->getType(), self_p->getFunc());

        const map_t &self_data = self_p->get();
        const map_t &tunnel_data = tunnel_p->get();

        for (auto &val: *self_data) {
            PMaybe tunnel1(nullptr);

            val.second.call<true>(caller, lib, tunnel1);

            tunnel_data->insert({{val.first, PVal(tunnel1)}});
        }

        tunnel = tunnel_p;
    }
_COTL_FUNC_END

namespace {

_COTL_FUNC_T(init)
_COTL_FUNC_BEGIN
    _COTL_CHECK_TUNNEL(false);

    PMaybe tunnel1(
        _libmap(
            id_atom, _libfunc(stdWrapAtom),
            id_int, _libfunc(stdWrapInt),
            id_real, _libfunc(stdWrapReal),
            id_func, _libfunc(stdWrapFunc),
            id_ptr, _libfunc(stdWrapPtr),
            id_pair, _libfunc(stdWrapPair),
            id_str, _libfunc(stdWrapStr),
            id_arr, _libfunc(stdWrapArr),
            id_map, _libfunc(stdWrapMap)
            // id_error, _libfunc(stdWrapErr)
        )
    );

    libSet<id_std, id_type>(id_type_wrap, lib, tunnel1);
_COTL_FUNC_END

long long ago = addInitializer(init, init_core_1);

}

}

}
