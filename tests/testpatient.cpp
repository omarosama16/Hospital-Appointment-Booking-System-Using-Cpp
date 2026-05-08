#include <gtest/gtest.h>
#include "Patient.h"

/* ================= CONSTRUCTOR - ALL FIELDS ================= */

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
    EXPECT_EQ(p.get_id(),       99);
    EXPECT_EQ(p.get_name(),     "Sara");
    EXPECT_EQ(p.get_email(),    "s@test.com");
    EXPECT_EQ(p.get_password(), "pass99");
    EXPECT_EQ(p.get_role(),     "patient");
    EXPECT_EQ(p.get_phone(),    "011");
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

/* ================= ROLE IS ALWAYS PATIENT ================= */

TEST(PatientTest, RoleIsAlwaysPatient)
{
    Patient p1(1, "A", "a@test.com", "p1", "010");
    Patient p2(2, "B", "b@test.com", "p2", "011");
    Patient p3(3, "C", "c@test.com", "p3", "012");

    EXPECT_EQ(p1.get_role(), "patient");
    EXPECT_EQ(p2.get_role(), "patient");
    EXPECT_EQ(p3.get_role(), "patient");
}

/* ================= PHONE SETTER / GETTER ================= */

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

TEST(PatientTest, SetPhone_ToLongNumber)
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

/* ================= AUTHENTICATE - ALL CONDITION BRANCHES ================= */

// email=TRUE && password=TRUE → true
TEST(PatientTest, Authenticate_Success)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    EXPECT_TRUE(p.Authenticate("a@test.com", "1234"));
}

// email=TRUE && password=FALSE → false
TEST(PatientTest, Authenticate_WrongPassword)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    EXPECT_FALSE(p.Authenticate("a@test.com", "wrong"));
}

// email=FALSE (short-circuit) → false
TEST(PatientTest, Authenticate_WrongEmail)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    EXPECT_FALSE(p.Authenticate("wrong@test.com", "1234"));
}

// email=FALSE && password=FALSE → false
TEST(PatientTest, Authenticate_BothWrong)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    EXPECT_FALSE(p.Authenticate("x", "y"));
}

// email=TRUE && password=FALSE: correct email, empty password
TEST(PatientTest, Authenticate_CorrectEmailEmptyPassword)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    EXPECT_FALSE(p.Authenticate("a@test.com", ""));
}

// email=FALSE: empty email with correct password
TEST(PatientTest, Authenticate_EmptyEmailCorrectPassword)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    EXPECT_FALSE(p.Authenticate("", "1234"));
}

// email=TRUE && password=TRUE on empty-string patient
TEST(PatientTest, Authenticate_EmptyCredentialsMatch)
{
    Patient p(1, "", "", "", "");
    EXPECT_TRUE(p.Authenticate("", ""));
}

// email=FALSE on empty-string patient
TEST(PatientTest, Authenticate_EmptyPatient_WrongEmail)
{
    Patient p(1, "", "", "", "");
    EXPECT_FALSE(p.Authenticate("someone@test.com", ""));
}

// email=TRUE && password=FALSE on empty-string patient
TEST(PatientTest, Authenticate_EmptyPatient_WrongPassword)
{
    Patient p(1, "", "", "", "");
    EXPECT_FALSE(p.Authenticate("", "wrongpass"));
}

/* ================= SETTERS (inherited from User) ================= */

TEST(PatientTest, AllSetters)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");

    p.set_id(55);
    p.set_name("NewName");
    p.set_email("new@test.com");
    p.set_password("newpass");
    p.set_role("patient");
    p.set_phone("099");

    EXPECT_EQ(p.get_id(),       55);
    EXPECT_EQ(p.get_name(),     "NewName");
    EXPECT_EQ(p.get_email(),    "new@test.com");
    EXPECT_EQ(p.get_password(), "newpass");
    EXPECT_EQ(p.get_role(),     "patient");
    EXPECT_EQ(p.get_phone(),    "099");
}

TEST(PatientTest, Authenticate_AfterSetEmail_Success)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    p.set_email("new@test.com");
    p.set_password("newpass");
    EXPECT_TRUE(p.Authenticate("new@test.com", "newpass"));
}

TEST(PatientTest, Authenticate_AfterSetEmail_WrongPassword)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    p.set_email("new@test.com");
    p.set_password("newpass");
    EXPECT_FALSE(p.Authenticate("new@test.com", "wrong"));
}

TEST(PatientTest, Authenticate_AfterSetEmail_WrongEmail)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    p.set_email("new@test.com");
    p.set_password("newpass");
    EXPECT_FALSE(p.Authenticate("other@test.com", "newpass"));
}

TEST(PatientTest, Authenticate_OldCredentialsFailAfterUpdate)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    p.set_email("new@test.com");
    p.set_password("newpass");
    EXPECT_FALSE(p.Authenticate("a@test.com", "1234"));
}

/* ================= MULTIPLE INSTANCES ================= */

TEST(PatientTest, MultipleInstances_Independent)
{
    Patient p1(1, "Ali",  "a@test.com", "pass1", "010");
    Patient p2(2, "Sara", "s@test.com", "pass2", "011");
    Patient p3(3, "Omar", "o@test.com", "pass3", "012");

    EXPECT_TRUE(p1.Authenticate("a@test.com", "pass1"));
    EXPECT_TRUE(p2.Authenticate("s@test.com", "pass2"));
    EXPECT_TRUE(p3.Authenticate("o@test.com", "pass3"));

    // Cross-auth fails
    EXPECT_FALSE(p1.Authenticate("s@test.com", "pass2"));
    EXPECT_FALSE(p2.Authenticate("a@test.com", "pass1"));

    // Phones independent
    EXPECT_EQ(p1.get_phone(), "010");
    EXPECT_EQ(p2.get_phone(), "011");
    EXPECT_EQ(p3.get_phone(), "012");
}

/* ================= STRESS ================= */

TEST(PatientTest, AuthenticateStress)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");
    for (int i = 0; i < 100; i++)
    {
        EXPECT_TRUE(p.Authenticate("a@test.com", "1234"));
        EXPECT_FALSE(p.Authenticate("a@test.com", "wrong"));
        EXPECT_FALSE(p.Authenticate("wrong@test.com", "1234"));
    }
}
