#include "doctest/doctest.h"

#include "../src/utils/genvec.h"

#include <optional>
#include <string>

TEST_SUITE("gen_vec") {
    using utils::gen_vec::GenIndex;
    using utils::gen_vec::GenVec;

    TEST_CASE("values_returned") {
        GenVec<std::string> vec;

        const GenIndex a = vec.insert("a");
        const GenIndex b = vec.insert("b");
        const GenIndex c = vec.insert("c");
        CHECK_EQ(vec.get(a).value().get(), "a");
        CHECK_EQ(vec.get(b).value().get(), "b");
        CHECK_EQ(vec.get(c).value().get(), "c");
    }

    TEST_CASE("values_removed") {
        GenVec<std::string> vec;

        const GenIndex a = vec.insert("a");
        vec.insert("b");
        vec.insert("c");

        vec.remove(a);

        CHECK_FALSE(vec.get(a).has_value());
        CHECK_EQ(vec.size(), 2);
    }

    TEST_CASE("values_reinserted") {
        GenVec<std::string> vec;

        const GenIndex a = vec.insert("a");
        vec.insert("b");
        vec.insert("c");

        vec.remove(a);

        const GenIndex d = vec.insert("d");

        CHECK_EQ(a.index, d.index);
        CHECK_NE(a.generation, d.generation);
    }

    TEST_CASE("remove_and_re_re_insert") {
        GenVec<std::string> vec;

        const GenIndex a = vec.insert("a");
        vec.insert("b");
        vec.insert("c");

        vec.remove(a);

        const GenIndex d = vec.insert("d");

        vec.remove(d);

        const GenIndex e = vec.insert("e");
        CHECK_EQ(a.index, e.index);
        CHECK_NE(a.generation, e.generation);
    }
}
