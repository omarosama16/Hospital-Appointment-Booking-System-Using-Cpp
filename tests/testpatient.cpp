#include <gtest/gtest.h>
#include "Patient.h"

/* ================= CONSTRUCTOR ================= */

TEST(PatientTest, Constructor_AllFields)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    EXPECT_EQ(p.get_id(),       1);
    EXPECT_EQ(p.get_name(),     "Ali");
    EXPECT_EQ(p.get_email(),    "a@test.com");
    EXPECT_EQ(p.get_password(), "1234");
    EXPECT_EQ(p.get_role(),     "patient");
    EXPECT_EQ(p.get_phone(),    "010");
}

TEST(PatientTest, Constructor_DifferentId)
{
    Patient p(99, "Sara", "s@test.com", "pass99", "011");
    EXPECT_EQ(p.get_id(),    99);
    EXPECT_EQ(p.get_role(),  "patient");
    EXPECT_EQ(p.get_phone(), "011");
}

TEST(PatientTest, Constructor_ZeroId)
{
    Patient p(0, "Zero", "z@test.com", "zpass", "000");
    EXPECT_EQ(p.get_id(),    0);
    EXPECT_EQ(p.get_role(),  "patient");
    EXPECT_EQ(p.get_phone(), "000");
}

TEST(PatientTest, Constructor_NegativeId)
{
    Patient p(-1, "Neg", "n@test.com", "npass", "999");
    EXPECT_EQ(p.get_id(),    -1);
    EXPECT_EQ(p.get_role(),  "patient");
    EXPECT_EQ(p.get_phone(), "999");
}

TEST(PatientTest, Constructor_EmptyStrings)
{
    Patient p(1, "", "", "", "");
    EXPECT_EQ(p.get_name(),     "");
    EXPECT_EQ(p.get_email(),    "");
    EXPECT_EQ(p.get_password(), "");
    EXPECT_EQ(p.get_role(),     "patient");
    EXPECT_EQ(p.get_phone(),    "");
}

TEST(PatientTest, Constructor_LongStrings)
{
    std::string longName(200, 'A');
    std::string longEmail(200, 'B');
    std::string longPass(200, 'C');
    std::string longPhone(20, '9');
    Patient p(42, longName, longEmail, longPass, longPhone);
    EXPECT_EQ(p.get_name(),     longName);
    EXPECT_EQ(p.get_email(),    longEmail);
    EXPECT_EQ(p.get_password(), longPass);
    EXPECT_EQ(p.get_phone(),    longPhone);
    EXPECT_EQ(p.get_role(),     "patient");
}

TEST(PatientTest, Constructor_SpecialCharacters)
{
    Patient p(5, "O'Brien", "o+tag@test.com", "p@$$!", "+20-010-999");
    EXPECT_EQ(p.get_name(),     "O'Brien");
    EXPECT_EQ(p.get_email(),    "o+tag@test.com");
    EXPECT_EQ(p.get_password(), "p@$$!");
    EXPECT_EQ(p.get_phone(),    "+20-010-999");
    EXPECT_EQ(p.get_role(),     "patient");
}

/* ================= COPY / MOVE OPERATIONS - FIX FOR 2 UNCOVERED CONDITIONS ================= */

// Copy constructor - covers compiler-generated copy conditions
TEST(PatientTest, CopyConstructor)
{
    Patient p1(1, "Ali", "a@test.com", "1234", "010");
    Patient p2(p1);  // copy constructor
    EXPECT_EQ(p2.get_id(),        1);
    EXPECT_EQ(p2.get_name(),      "Ali");
    EXPECT_EQ(p2.get_email(),     "a@test.com");
    EXPECT_EQ(p2.get_password(),  "1234");
    EXPECT_EQ(p2.get_phone(),     "010");
    EXPECT_EQ(p2.get_role(),      "patient");

    // Verify independent copies
    p2.set_phone("999");
    EXPECT_EQ(p1.get_phone(), "010");
    EXPECT_EQ(p2.get_phone(), "999");
}

// Copy assignment - covers compiler-generated copy assignment conditions
TEST(PatientTest, CopyAssignment)
{
    Patient p1(1, "Ali", "a@test.com", "1234", "010");
    Patient p2(2, "Sara", "s@test.com", "5678", "011");
    p2 = p1;  // copy assignment
    EXPECT_EQ(p2.get_id(),        1);
    EXPECT_EQ(p2.get_name(),      "Ali");
    EXPECT_EQ(p2.get_email(),     "a@test.com");
    EXPECT_EQ(p2.get_password(),  "1234");
    EXPECT_EQ(p2.get_phone(),     "010");
    EXPECT_EQ(p2.get_role(),      "patient");
}

// Move constructor - covers compiler-generated move conditions
TEST(PatientTest, MoveConstructor)
{
    Patient p1(1, "Ali", "a@test.com", "1234", "010");
    Patient p2(std::move(p1));  // move constructor
    EXPECT_EQ(p2.get_id(),       1);
    EXPECT_EQ(p2.get_name(),     "Ali");
    EXPECT_EQ(p2.get_email(),    "a@test.com");
    EXPECT_EQ(p2.get_password(), "1234");
    EXPECT_EQ(p2.get_phone(),    "010");
    EXPECT_EQ(p2.get_role(),     "patient");
}

// Move assignment - covers compiler-generated move assignment conditions
TEST(PatientTest, MoveAssignment)
{
    Patient p1(1, "Ali", "a@test.com", "1234", "010");
    Patient p2(2, "Sara", "s@test.com", "5678", "011");
    p2 = std::move(p1);  // move assignment
    EXPECT_EQ(p2.get_id(),       1);
    EXPECT_EQ(p2.get_name(),     "Ali");
    EXPECT_EQ(p2.get_email(),    "a@test.com");
    EXPECT_EQ(p2.get_password(), "1234");
    EXPECT_EQ(p2.get_phone(),    "010");
    EXPECT_EQ(p2.get_role(),     "patient");
}

/* ================= ROLE IS ALWAYS PATIENT ================= */

TEST(PatientTest, RoleIsAlwaysPatient)
{
    Patient p1(1, "A", "a@test.com", "p1", "010");
    Patient p2(2, "B", "b@test.com", "p2", "011");
    EXPECT_EQ(p1.get_role(), "patient");
    EXPECT_EQ(p2.get_role(), "patient");
}

/* ================= PHONE SETTER/GETTER ================= */

TEST(PatientTest, SetPhone_Basic)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    p.set_phone("011");
    EXPECT_EQ(p.get_phone(), "011");
}

TEST(PatientTest, SetPhone_ToEmpty)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    p.set_phone("");
    EXPECT_EQ(p.get_phone(), "");
}

TEST(PatientTest, SetPhone_LongNumber)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    std::string longPhone(30, '5');
    p.set_phone(longPhone);
    EXPECT_EQ(p.get_phone(), longPhone);
}

TEST(PatientTest, SetPhone_MultipleUpdates)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    p.set_phone("011");
    EXPECT_EQ(p.get_phone(), "011");
    p.set_phone("012");
    EXPECT_EQ(p.get_phone(), "012");
    p.set_phone("010");
    EXPECT_EQ(p.get_phone(), "010");
}

TEST(PatientTest, SetPhone_SpecialChars)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    p.set_phone("+20-010-123-4567");
    EXPECT_EQ(p.get_phone(), "+20-010-123-4567");
}

/* ================= AUTHENTICATE - ALL BRANCHES ================= */

TEST(PatientTest, Auth_Success)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    EXPECT_TRUE(p.Authenticate("a@test.com", "1234"));
}

TEST(PatientTest, Auth_WrongPassword)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    EXPECT_FALSE(p.Authenticate("a@test.com", "wrong"));
}

TEST(PatientTest, Auth_WrongEmail)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    EXPECT_FALSE(p.Authenticate("wrong@test.com", "1234"));
}

TEST(PatientTest, Auth_BothWrong)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    EXPECT_FALSE(p.Authenticate("x", "y"));
}

TEST(PatientTest, Auth_CorrectEmailEmptyPassword)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    EXPECT_FALSE(p.Authenticate("a@test.com", ""));
}

TEST(PatientTest, Auth_EmptyEmailCorrectPassword)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    EXPECT_FALSE(p.Authenticate("", "1234"));
}

TEST(PatientTest, Auth_EmptyCredentialsMatch)
{
    Patient p(1, "", "", "", "");
    EXPECT_TRUE(p.Authenticate("", ""));
}

TEST(PatientTest, Auth_EmptyPatient_WrongEmail)
{
    Patient p(1, "", "", "", "");
    EXPECT_FALSE(p.Authenticate("x@test.com", ""));
}

TEST(PatientTest, Auth_EmptyPatient_WrongPassword)
{
    Patient p(1, "", "", "", "");
    EXPECT_FALSE(p.Authenticate("", "wrongpass"));
}

/* ================= SETTERS ================= */

TEST(PatientTest, AllSetters)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    p.set_id(55);
    p.set_name("New");
    p.set_email("new@test.com");
    p.set_password("newpass");
    p.set_role("patient");
    p.set_phone("099");
    EXPECT_EQ(p.get_id(),       55);
    EXPECT_EQ(p.get_name(),     "New");
    EXPECT_EQ(p.get_email(),    "new@test.com");
    EXPECT_EQ(p.get_password(), "newpass");
    EXPECT_EQ(p.get_role(),     "patient");
    EXPECT_EQ(p.get_phone(),    "099");
}

TEST(PatientTest, Auth_AfterSetters_Success)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    p.set_email("new@test.com");
    p.set_password("newpass");
    EXPECT_TRUE(p.Authenticate("new@test.com", "newpass"));
}

TEST(PatientTest, Auth_AfterSetters_WrongPassword)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    p.set_email("new@test.com");
    p.set_password("newpass");
    EXPECT_FALSE(p.Authenticate("new@test.com", "wrong"));
}

TEST(PatientTest, Auth_AfterSetters_WrongEmail)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    p.set_email("new@test.com");
    p.set_password("newpass");
    EXPECT_FALSE(p.Authenticate("other@test.com", "newpass"));
}

TEST(PatientTest, Auth_OldCredsFail)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    p.set_email("new@test.com");
    p.set_password("newpass");
    EXPECT_FALSE(p.Authenticate("a@test.com", "1234"));
}

/* ================= MULTIPLE INSTANCES ================= */

TEST(PatientTest, MultipleInstances)
{
    Patient p1(1, "Ali",  "a@test.com", "pass1", "010");
    Patient p2(2, "Sara", "s@test.com", "pass2", "011");
    Patient p3(3, "Omar", "o@test.com", "pass3", "012");

    EXPECT_TRUE(p1.Authenticate("a@test.com", "pass1"));
    EXPECT_TRUE(p2.Authenticate("s@test.com", "pass2"));
    EXPECT_TRUE(p3.Authenticate("o@test.com", "pass3"));

    EXPECT_FALSE(p1.Authenticate("s@test.com", "pass2"));
    EXPECT_FALSE(p2.Authenticate("a@test.com", "pass1"));

    EXPECT_EQ(p1.get_phone(), "010");
    EXPECT_EQ(p2.get_phone(), "011");
    EXPECT_EQ(p3.get_phone(), "012");
}

/* ================= STRESS ================= */

TEST(PatientTest, AuthStress)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    for (int i = 0; i < 50; i++)
    {
        EXPECT_TRUE(p.Authenticate("a@test.com", "1234"));
        EXPECT_FALSE(p.Authenticate("a@test.com", "wrong"));
        EXPECT_FALSE(p.Authenticate("wrong@test.com", "1234"));
    }
}
