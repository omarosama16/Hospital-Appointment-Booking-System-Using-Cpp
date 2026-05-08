#include <gtest/gtest.h>
#include "Doctor.h"
#include <limits>

/* ================= CONSTRUCTOR - ALL BRANCHES ================= */

TEST(DoctorTest, Constructor_AllFields_Valid)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    EXPECT_EQ(d.get_id(),            1);
    EXPECT_EQ(d.get_name(),          "Omar");
    EXPECT_EQ(d.get_email(),         "o@test.com");
    EXPECT_EQ(d.get_password(),      "1234");
    EXPECT_EQ(d.get_role(),          "doctor");
    EXPECT_EQ(d.getSpecialization(), "Cardiology");
}

TEST(DoctorTest, Constructor_DifferentId)
{
    Doctor d(99, "Sara", "s@test.com", "pass99", "Neurology");
    EXPECT_EQ(d.get_id(),            99);
    EXPECT_EQ(d.get_role(),          "doctor");
    EXPECT_EQ(d.getSpecialization(), "Neurology");
}

TEST(DoctorTest, Constructor_ZeroId)
{
    // Covers: User constructor branch for id == 0
    Doctor d(0, "Zero", "z@test.com", "zpass", "Dermatology");
    EXPECT_EQ(d.get_id(),            0);
    EXPECT_EQ(d.get_role(),          "doctor");
    EXPECT_EQ(d.getSpecialization(), "Dermatology");
}

TEST(DoctorTest, Constructor_NegativeId)
{
    // Covers: User constructor branch for id < 0
    Doctor d(-1, "Neg", "n@test.com", "npass", "Pediatrics");
    EXPECT_EQ(d.get_id(),            -1);
    EXPECT_EQ(d.get_role(),          "doctor");
    EXPECT_EQ(d.getSpecialization(), "Pediatrics");
}

TEST(DoctorTest, Constructor_MinIntId)
{
    // Covers: User constructor extreme negative boundary
    Doctor d(std::numeric_limits<int>::min(), "Min", "min@test.com", "minpass", "Surgery");
    EXPECT_EQ(d.get_id(), std::numeric_limits<int>::min());
    EXPECT_EQ(d.get_role(), "doctor");
}

TEST(DoctorTest, Constructor_MaxIntId)
{
    // Covers: User constructor extreme positive boundary
    Doctor d(std::numeric_limits<int>::max(), "Max", "max@test.com", "maxpass", "Radiology");
    EXPECT_EQ(d.get_id(), std::numeric_limits<int>::max());
    EXPECT_EQ(d.get_role(), "doctor");
}

TEST(DoctorTest, Constructor_EmptyName)
{
    // Covers: User constructor branch for empty name
    Doctor d(1, "", "email@test.com", "pass", "Cardiology");
    EXPECT_EQ(d.get_name(), "");
    EXPECT_EQ(d.get_role(), "doctor");
}

TEST(DoctorTest, Constructor_EmptyEmail)
{
    // Covers: User constructor branch for empty email
    Doctor d(1, "Name", "", "pass", "Cardiology");
    EXPECT_EQ(d.get_email(), "");
    EXPECT_EQ(d.get_role(), "doctor");
}

TEST(DoctorTest, Constructor_EmptyPassword)
{
    // Covers: User constructor branch for empty password
    Doctor d(1, "Name", "email@test.com", "", "Cardiology");
    EXPECT_EQ(d.get_password(), "");
    EXPECT_EQ(d.get_role(), "doctor");
}

TEST(DoctorTest, Constructor_EmptySpecialization)
{
    // Covers: empty specialization (Doctor field, not User)
    Doctor d(1, "Name", "email@test.com", "pass", "");
    EXPECT_EQ(d.getSpecialization(), "");
    EXPECT_EQ(d.get_role(), "doctor");
}

TEST(DoctorTest, Constructor_AllEmptyStrings)
{
    // Covers: User constructor all-empty branch
    Doctor d(1, "", "", "", "");
    EXPECT_EQ(d.get_name(),     "");
    EXPECT_EQ(d.get_email(),    "");
    EXPECT_EQ(d.get_password(), "");
    EXPECT_EQ(d.get_role(),     "doctor");
    EXPECT_EQ(d.getSpecialization(), "");
}

TEST(DoctorTest, Constructor_LongStrings)
{
    std::string longName(200, 'A');
    std::string longEmail(200, 'B');
    std::string longPass(200, 'C');
    std::string longSpec(200, 'D');
    Doctor d(42, longName, longEmail, longPass, longSpec);
    EXPECT_EQ(d.get_name(),          longName);
    EXPECT_EQ(d.get_email(),         longEmail);
    EXPECT_EQ(d.get_password(),      longPass);
    EXPECT_EQ(d.getSpecialization(), longSpec);
    EXPECT_EQ(d.get_role(),          "doctor");
}

TEST(DoctorTest, Constructor_SpecialCharacters)
{
    Doctor d(5, "O'Brien", "o+tag@test.com", "p@$$!", "Cardio-Thoracic");
    EXPECT_EQ(d.get_name(),          "O'Brien");
    EXPECT_EQ(d.get_email(),         "o+tag@test.com");
    EXPECT_EQ(d.get_password(),      "p@$$!");
    EXPECT_EQ(d.getSpecialization(), "Cardio-Thoracic");
    EXPECT_EQ(d.get_role(),          "doctor");
}

TEST(DoctorTest, Constructor_EmailNoAtSymbol)
{
    // Covers: User constructor email validation branch (invalid format)
    Doctor d(1, "Name", "invalid-email", "pass", "Cardiology");
    EXPECT_EQ(d.get_email(), "invalid-email");
    // If User validates email, this might throw or set empty — adjust expectation based on implementation
}

TEST(DoctorTest, Constructor_EmailMultipleAtSymbols)
{
    // Covers: User constructor email validation branch (multiple @)
    Doctor d(1, "Name", "a@b@c.com", "pass", "Cardiology");
    EXPECT_EQ(d.get_email(), "a@b@c.com");
}

TEST(DoctorTest, Constructor_EmailNoDomain)
{
    // Covers: User constructor email validation branch (no domain after @)
    Doctor d(1, "Name", "name@", "pass", "Cardiology");
    EXPECT_EQ(d.get_email(), "name@");
}

TEST(DoctorTest, Constructor_WhitespaceInFields)
{
    Doctor d(1, "  Name  ", "  email@test.com  ", "  pass  ", "  Spec  ");
    EXPECT_EQ(d.get_name(),     "  Name  ");
    EXPECT_EQ(d.get_email(),    "  email@test.com  ");
    EXPECT_EQ(d.get_password(), "  pass  ");
    EXPECT_EQ(d.getSpecialization(), "  Spec  ");
}

/* ================= ROLE IS ALWAYS DOCTOR ================= */

TEST(DoctorTest, RoleIsAlwaysDoctor_MultipleInstances)
{
    Doctor d1(1, "A", "a@test.com", "p1", "Cardiology");
    Doctor d2(2, "B", "b@test.com", "p2", "Neurology");
    Doctor d3(-5, "C", "c@test.com", "p3", "Surgery");
    EXPECT_EQ(d1.get_role(), "doctor");
    EXPECT_EQ(d2.get_role(), "doctor");
    EXPECT_EQ(d3.get_role(), "doctor");
}

/* ================= SPECIALIZATION ================= */

TEST(DoctorTest, Specialization_Various)
{
    Doctor d1(1, "A", "a@test.com", "p", "Cardiology");
    Doctor d2(2, "B", "b@test.com", "p", "Neurology");
    Doctor d3(3, "C", "c@test.com", "p", "Orthopedics");
    Doctor d4(4, "D", "d@test.com", "p", "");
    EXPECT_EQ(d1.getSpecialization(), "Cardiology");
    EXPECT_EQ(d2.getSpecialization(), "Neurology");
    EXPECT_EQ(d3.getSpecialization(), "Orthopedics");
    EXPECT_EQ(d4.getSpecialization(), "");
}

/* ================= AVAILABILITY ================= */

TEST(DoctorTest, Availability_StartsEmpty)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    EXPECT_TRUE(d.getAvailability().empty());
}

TEST(DoctorTest, Availability_AddOne)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    d.addAvailability("10AM");
    EXPECT_EQ(d.getAvailability().size(), 1u);
    EXPECT_EQ(d.getAvailability()[0], "10AM");
}

TEST(DoctorTest, Availability_AddMultiple)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    d.addAvailability("10AM");
    d.addAvailability("11AM");
    d.addAvailability("2PM");
    EXPECT_EQ(d.getAvailability().size(), 3u);
    EXPECT_EQ(d.getAvailability()[0], "10AM");
    EXPECT_EQ(d.getAvailability()[1], "11AM");
    EXPECT_EQ(d.getAvailability()[2], "2PM");
}

TEST(DoctorTest, Availability_AddDuplicates)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    d.addAvailability("10AM");
    d.addAvailability("10AM");
    EXPECT_EQ(d.getAvailability().size(), 2u);
    EXPECT_EQ(d.getAvailability()[0], "10AM");
    EXPECT_EQ(d.getAvailability()[1], "10AM");
}

TEST(DoctorTest, Availability_ClearAfterAdd)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    d.addAvailability("10AM");
    d.addAvailability("11AM");
    d.clearAvailability();
    EXPECT_TRUE(d.getAvailability().empty());
}

TEST(DoctorTest, Availability_ClearEmpty)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    EXPECT_NO_THROW(d.clearAvailability());
    EXPECT_TRUE(d.getAvailability().empty());
}

TEST(DoctorTest, Availability_AddAfterClear)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    d.addAvailability("10AM");
    d.clearAvailability();
    d.addAvailability("3PM");
    EXPECT_EQ(d.getAvailability().size(), 1u);
    EXPECT_EQ(d.getAvailability()[0], "3PM");
}

TEST(DoctorTest, Availability_MultipleClearCycles)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    for (int i = 0; i < 3; i++)
    {
        d.addAvailability("10AM");
        d.addAvailability("11AM");
        EXPECT_EQ(d.getAvailability().size(), 2u);
        d.clearAvailability();
        EXPECT_TRUE(d.getAvailability().empty());
    }
}

TEST(DoctorTest, Availability_EmptyString)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    d.addAvailability("");
    EXPECT_EQ(d.getAvailability().size(), 1u);
    EXPECT_EQ(d.getAvailability()[0], "");
}

TEST(DoctorTest, Availability_ManySlots)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    for (int i = 0; i < 100; i++)
        d.addAvailability("slot" + std::to_string(i));
    EXPECT_EQ(d.getAvailability().size(), 100u);
    d.clearAvailability();
    EXPECT_EQ(d.getAvailability().size(), 0u);
}

TEST(DoctorTest, Availability_LongStringSlot)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    std::string longSlot(500, 'X');
    d.addAvailability(longSlot);
    EXPECT_EQ(d.getAvailability().size(), 1u);
    EXPECT_EQ(d.getAvailability()[0], longSlot);
}

/* ================= AUTHENTICATE - ALL 4 BRANCHES ================= */

TEST(DoctorTest, Auth_Success)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    EXPECT_TRUE(d.Authenticate("o@test.com", "1234"));
}

TEST(DoctorTest, Auth_WrongPassword)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    EXPECT_FALSE(d.Authenticate("o@test.com", "wrong"));
}

TEST(DoctorTest, Auth_WrongEmail)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    EXPECT_FALSE(d.Authenticate("wrong@test.com", "1234"));
}

TEST(DoctorTest, Auth_BothWrong)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    EXPECT_FALSE(d.Authenticate("x", "y"));
}

TEST(DoctorTest, Auth_CorrectEmailEmptyPassword)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    EXPECT_FALSE(d.Authenticate("o@test.com", ""));
}

TEST(DoctorTest, Auth_EmptyEmailCorrectPassword)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    EXPECT_FALSE(d.Authenticate("", "1234"));
}

TEST(DoctorTest, Auth_EmptyCredentialsMatch)
{
    // Covers: User constructor + Auth with all empty fields
    Doctor d(1, "", "", "", "");
    EXPECT_TRUE(d.Authenticate("", ""));
}

TEST(DoctorTest, Auth_EmptyDoctor_WrongEmail)
{
    Doctor d(1, "", "", "", "");
    EXPECT_FALSE(d.Authenticate("x@test.com", ""));
}

TEST(DoctorTest, Auth_EmptyDoctor_WrongPassword)
{
    Doctor d(1, "", "", "", "");
    EXPECT_FALSE(d.Authenticate("", "wrongpass"));
}

TEST(DoctorTest, Auth_EmptyDoctor_BothWrong)
{
    Doctor d(1, "", "", "", "");
    EXPECT_FALSE(d.Authenticate("x", "y"));
}

TEST(DoctorTest, Auth_CaseSensitiveEmail)
{
    Doctor d(1, "Omar", "Omar@Test.Com", "1234", "Cardiology");
    EXPECT_FALSE(d.Authenticate("omar@test.com", "1234"));
    EXPECT_TRUE(d.Authenticate("Omar@Test.Com", "1234"));
}

TEST(DoctorTest, Auth_CaseSensitivePassword)
{
    Doctor d(1, "Omar", "o@test.com", "PassWord", "Cardiology");
    EXPECT_FALSE(d.Authenticate("o@test.com", "password"));
    EXPECT_TRUE(d.Authenticate("o@test.com", "PassWord"));
}

/* ================= SETTERS ================= */

TEST(DoctorTest, AllSetters)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    d.set_id(55);
    d.set_name("New");
    d.set_email("new@test.com");
    d.set_password("newpass");
    EXPECT_EQ(d.get_id(),       55);
    EXPECT_EQ(d.get_name(),     "New");
    EXPECT_EQ(d.get_email(),    "new@test.com");
    EXPECT_EQ(d.get_password(), "newpass");
    EXPECT_EQ(d.get_role(),     "doctor");
}

TEST(DoctorTest, Setters_BoundaryValues)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    d.set_id(0);
    EXPECT_EQ(d.get_id(), 0);
    d.set_id(-999);
    EXPECT_EQ(d.get_id(), -999);
    d.set_id(std::numeric_limits<int>::max());
    EXPECT_EQ(d.get_id(), std::numeric_limits<int>::max());
}

TEST(DoctorTest, Auth_AfterSetters_Success)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    d.set_email("new@test.com");
    d.set_password("newpass");
    EXPECT_TRUE(d.Authenticate("new@test.com", "newpass"));
}

TEST(DoctorTest, Auth_AfterSetters_WrongPassword)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    d.set_email("new@test.com");
    d.set_password("newpass");
    EXPECT_FALSE(d.Authenticate("new@test.com", "wrong"));
}

TEST(DoctorTest, Auth_AfterSetters_WrongEmail)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    d.set_email("new@test.com");
    d.set_password("newpass");
    EXPECT_FALSE(d.Authenticate("other@test.com", "newpass"));
}

TEST(DoctorTest, Auth_OldCredsFail)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    d.set_email("new@test.com");
    d.set_password("newpass");
    EXPECT_FALSE(d.Authenticate("o@test.com", "1234"));
}

/* ================= MULTIPLE INSTANCES ================= */

TEST(DoctorTest, MultipleInstances)
{
    Doctor d1(1, "Ali",  "a@test.com", "pass1", "Cardiology");
    Doctor d2(2, "Sara", "s@test.com", "pass2", "Neurology");
    Doctor d3(3, "Omar", "o@test.com", "pass3", "Pediatrics");

    EXPECT_TRUE(d1.Authenticate("a@test.com", "pass1"));
    EXPECT_TRUE(d2.Authenticate("s@test.com", "pass2"));
    EXPECT_TRUE(d3.Authenticate("o@test.com", "pass3"));

    EXPECT_FALSE(d1.Authenticate("s@test.com", "pass2"));
    EXPECT_FALSE(d2.Authenticate("a@test.com", "pass1"));

    EXPECT_EQ(d1.getSpecialization(), "Cardiology");
    EXPECT_EQ(d2.getSpecialization(), "Neurology");
    EXPECT_EQ(d3.getSpecialization(), "Pediatrics");
}

/* ================= STRESS ================= */

TEST(DoctorTest, AuthStress)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    for (int i = 0; i < 100; i++)
    {
        EXPECT_TRUE(d.Authenticate("o@test.com", "1234"));
        EXPECT_FALSE(d.Authenticate("o@test.com", "wrong"));
        EXPECT_FALSE(d.Authenticate("wrong@test.com", "1234"));
        EXPECT_FALSE(d.Authenticate("", ""));
    }
}

TEST(DoctorTest, AvailabilityStress)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");
    for (int i = 0; i < 1000; i++)
    {
        d.addAvailability(std::to_string(i));
    }
    EXPECT_EQ(d.getAvailability().size(), 1000u);
    d.clearAvailability();
    EXPECT_TRUE(d.getAvailability().empty());
}
