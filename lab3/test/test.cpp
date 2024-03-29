#include <gtest/gtest.h>
#include <string>
#include "../include/word.hpp"

TEST(WordConstructor, DefaultConstructor)
{
    word::words tmp;
    ASSERT_EQ(0, tmp.get_count());
}

TEST(WordConstructor, WordConstructor)
{
    word::words tmp("Hi");
    char *str;
    ASSERT_EQ(1, tmp.get_count());
    str = tmp.get_word(0);
    std::cout << str << std::endl;
    ASSERT_STREQ("Hi", str);
    delete str;
    ASSERT_ANY_THROW(word::words tmp_1("Toomanylenofthiswordsoitwillbedenied"));
}

TEST(WordConstructor, MassConstructor)
{
    const char *mass[4] = {"ABC", "HI", "HALLO", "PROST"};
    unsigned n = 4;
    word::words tmp(n, mass);
    ASSERT_EQ(4, tmp.get_count());
    ASSERT_STREQ("ABC", tmp.get_word(0));
    ASSERT_STREQ("HI", tmp.get_word(1));
    ASSERT_STREQ("HALLO", tmp.get_word(2));
    ASSERT_STREQ("PROST", tmp.get_word(3));
}

TEST(WordMethod, Get_count)
{
    word::words tmp("ABC");
    ASSERT_EQ(1, tmp.get_count());
    tmp.add("Hi");
    ASSERT_EQ(2, tmp.get_count());
    tmp.add("Hello");
    ASSERT_EQ(3, tmp.get_count());
    tmp.add("Hallo");
    ASSERT_EQ(4, tmp.get_count());
    tmp.add("PROST");
}

TEST(WordMethod, Add)
{
    word::words tmp;
    tmp.add("Kukuku");
    tmp.add("Kakaka");
    tmp.add("Kikiki");
    ASSERT_STREQ("Kukuku", tmp.get_word(0));
    ASSERT_STREQ("Kakaka", tmp.get_word(1));
    ASSERT_STREQ("Kikiki", tmp.get_word(2));
    ASSERT_ANY_THROW(tmp.add("Kukuku"));
    ASSERT_ANY_THROW(tmp.add("Toomanylenofthiswordsoitwillbedenied"));
}

TEST(WordMethod, Clear)
{
    word::words tmp("ABC");
    tmp.add("Hi");
    tmp.add("Hello");
    tmp.add("Hallo");
    tmp.add("PROST");
    tmp.clear();
    ASSERT_EQ(0, tmp.get_count());
}

TEST(WordMethod, Delete)
{
    word::words tmp("ABC");
    tmp.add("Hi");
    tmp.add("Hello");
    tmp.add("Hallo");
    tmp.add("PROST");
    tmp.del(2);
    ASSERT_EQ(4, tmp.get_count());
    ASSERT_STREQ("PROST", tmp.get_word(tmp.get_count() - 1));
    ASSERT_ANY_THROW(tmp.get_word(4));
    ASSERT_ANY_THROW(tmp.del(4));
    tmp.clear();
    ASSERT_ANY_THROW(tmp.del(0));
}

TEST(WordMethod, Search)
{
    word::words tmp("ABC");
    tmp.add("Hi");
    tmp.add("Hello");
    tmp.add("Hallo");
    tmp.add("PROST");
    ASSERT_EQ(1, tmp.search("Hi"));
    ASSERT_EQ(4, tmp.search("PROST"));
    ASSERT_EQ(tmp.get_count(), tmp.search("HAHAHAHAHAHAHA"));
}

TEST(WordMethod, Get_words)
{
    word::words tmp;
    tmp.add("Hi");
    tmp.add("Hello");
    tmp.add("Hallo");
    tmp.add("PROST");
    ASSERT_STREQ("Hi", tmp.get_word(0));
    ASSERT_STREQ("Hello", tmp.get_word(1));
    ASSERT_STREQ("Hallo", tmp.get_word(2));
    ASSERT_ANY_THROW(tmp.get_word(10));
}

TEST(WordMethod, First_symbol)
{
    word::words tmp_1;
    word::words tmp_2;
    tmp_1.add("Hi");
    tmp_1.add("hello");
    tmp_1.add("Hallo");
    tmp_1.add("PROST");
    tmp_2.add("Hi");
    tmp_2.add("hello");
    tmp_2.add("Hallo");
    word::words *h_mass;
    h_mass = tmp_1.first_symbol('H');
    ASSERT_STREQ(tmp_2.get_word(0), h_mass->get_word(0));
    ASSERT_STREQ(tmp_2.get_word(1), h_mass->get_word(1));
    ASSERT_STREQ(tmp_2.get_word(2), h_mass->get_word(2));
    delete h_mass;
    h_mass = tmp_1.first_symbol('h');
    ASSERT_STREQ(tmp_2.get_word(0), h_mass->get_word(0));
    ASSERT_STREQ(tmp_2.get_word(1), h_mass->get_word(1));
    ASSERT_STREQ(tmp_2.get_word(2), h_mass->get_word(2));
    delete h_mass;
}

TEST(WordMethod, Sort)
{
    word::words tmp_1("hallo");
    tmp_1.add("Hi");
    tmp_1.add("Hello");
    tmp_1.add("ABC");
    tmp_1.add("PROST");
    word::words tmp_2("ABC");
    tmp_2.add("hallo");
    tmp_2.add("Hello");
    tmp_2.add("Hi");
    tmp_2.add("PROST");
    tmp_1.sort();
    ASSERT_STREQ(tmp_2.get_word(0), tmp_1.get_word(0));
    ASSERT_STREQ(tmp_2.get_word(1), tmp_1.get_word(1));
    ASSERT_STREQ(tmp_2.get_word(2), tmp_1.get_word(2));
    ASSERT_STREQ(tmp_2.get_word(3), tmp_1.get_word(3));
    ASSERT_STREQ(tmp_2.get_word(4), tmp_1.get_word(4));
}

TEST(IO, Print)
{
    std::ostringstream ostr_1, ostr_2;
    word::words tmp;
    tmp.print(ostr_1);
    ASSERT_STREQ("Word Massive sizeof 10 is empty\n", ostr_1.str().c_str());
    tmp.add("Hi");
    tmp.add("Hello");
    tmp.print(ostr_2);
    ASSERT_STREQ("Word Massive sizeof 10 filled with 2 elements\n[ 'Hi', 'Hello' ]\n", ostr_2.str().c_str());
}

TEST(IO, Input)
{
    std::istringstream istr_1("KUKUKU\n");
    std::istringstream istr_2("Hi\n");
    word::words tmp;
    tmp.add("Hi");
    tmp.add("Hello");
    tmp.add("Hallo");
    tmp.add("PROST");
    tmp.input(istr_1);
    ASSERT_STREQ("KUKUKU", tmp.get_word(4));
    ASSERT_ANY_THROW(tmp.input(istr_2));
}

TEST(IO, Cout)
{
    std::ostringstream ostr_1, ostr_2;
    word::words tmp;
    ostr_1 << tmp;
    ASSERT_STREQ("Word Massive sizeof 10 is empty\n", ostr_1.str().c_str());
    tmp.add("Hi");
    tmp.add("Hello");
    ostr_2 << tmp;
    ASSERT_STREQ("Word Massive sizeof 10 filled with 2 elements\n[ 'Hi', 'Hello' ]\n", ostr_2.str().c_str());

}

TEST(IO, Cin)
{
    std::istringstream istr_1("KUKUKU\n");
    std::istringstream istr_2("Hi\n");
    word::words tmp;
    tmp.add("Hi");
    tmp.add("Hello");
    tmp.add("Hallo");
    tmp.add("PROST");
    istr_1 >> tmp;
    ASSERT_STREQ("KUKUKU", tmp.get_word(4));
    ASSERT_ANY_THROW(istr_2 >> tmp);
}

TEST(Operator, Decrement)
{
    word::words tmp;
    tmp.add("Kukuku");
    tmp.add("Kakaka");
    tmp.add("Kikiki");
    ASSERT_EQ(3, (tmp--).get_count());
    ASSERT_EQ(1, (--tmp).get_count());
    tmp--;
    ASSERT_ANY_THROW(tmp--);
}

TEST(Operator, Add)
{
    word::words tmp_1;
    tmp_1.add("Kukuku");
    tmp_1.add("Kakaka");
    tmp_1.add("Kikiki");
    word::words tmp_2;
    tmp_2.add("Kikooki");
    tmp_1 += tmp_2;
    ASSERT_EQ(4, tmp_1.get_count());
    ASSERT_EQ(1, tmp_2.get_count());
    tmp_2.add("Kikiki");
    ASSERT_ANY_THROW(tmp_1 += tmp_2);
}

TEST(Operator, IsEmpty)
{
    word::words tmp;
    ASSERT_EQ(true, !tmp);
    tmp.add("Kukuku");
    ASSERT_EQ(false, !tmp);
}

TEST(Operator, Sum)
{
    word::words tmp_1;
    tmp_1.add("Kukuku");
    tmp_1.add("Kakaka");
    tmp_1.add("Kikiki");
    word::words tmp_2(tmp_1);
    word::words sum;
    ASSERT_ANY_THROW(sum = tmp_1 + tmp_2);
    word::words tmp_3;
    tmp_3.add("HAHAHA");
    sum = tmp_1 + tmp_3;
    ASSERT_EQ(4, sum.get_count());
}

TEST(Operator, Compare)
{
    word::words tmp_1;
    tmp_1.add("Kukuku");
    tmp_1.add("Kakaka");
    tmp_1.add("Kikiki");
    word::words tmp_2(tmp_1);
    ASSERT_EQ(std::strong_ordering::equal, tmp_1<=>tmp_2);
    tmp_2.add("Hahaha");
    ASSERT_EQ(std::strong_ordering::less, tmp_1<=>tmp_2);
}

TEST(Operator, Indexes)
{
    word::words tmp;
    tmp.add("Kukuku");
    tmp.add("Kakaka");
    tmp.add("Kikiki");
    ASSERT_STREQ("Kukuku", tmp[0]);
    ASSERT_ANY_THROW(tmp[3]);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}