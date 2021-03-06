/**
 * OpenDaVINCI - Portable middleware for distributed components.
 * Copyright (C) 2015 Christian Berger
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef CORE_MESSAGETESTSUITE_H_
#define CORE_MESSAGETESTSUITE_H_

#include <cmath>                        // for fabs
#include <cstdlib>                      // for calloc
#include <cstring>                      // for strcmp
#include <iosfwd>                       // for stringstream, istream, etc
#include <memory>
#include <string>                       // for string

#include "cxxtest/TestSuite.h"          // for TS_ASSERT, TestSuite

#include "opendavinci/odcore/opendavinci.h"
#include "opendavinci/odcore/reflection/Field.h"
#include "opendavinci/odcore/serialization/Deserializer.h"     // for Deserializer
#include "opendavinci/odcore/serialization/Serializable.h"     // for Serializable
#include "opendavinci/odcore/serialization/SerializationFactory.h"  // for SerializationFactory
#include "opendavinci/odcore/serialization/Serializer.h"       // for Serializer
#include "opendavinci/odcore/base/Visitable.h"        // for Visitable
#include "opendavinci/odcore/base/Visitor.h"          // for Visitor
#include "opendavinci/odcore/reflection/Message.h"    // for Message
#include "opendavinci/odcore/reflection/MessageFromVisitableVisitor.h"
#include "opendavinci/odcore/reflection/MessagePrettyPrinterVisitor.h"
#include "opendavinci/odcore/reflection/MessageToVisitableVisitor.h"
#include "opendavinci/odcore/reflection/CSVFromVisitableVisitor.h"
#include "opendavinci/odcore/strings/StringToolbox.h"  // for StringToolbox
#include "opendavincitestdata/generated/odcore/testdata/TestMessage10.h"

using namespace std;
using namespace odcore;
using namespace odcore::base;
using namespace odcore::data;
using namespace odcore::reflection;
using namespace odcore::data::reflection;
using namespace odcore::serialization;
using namespace odcore::testdata;

class MyRawVisitable : public odcore::serialization::Serializable, public Visitable {
    public:
        MyRawVisitable() :
            data(NULL) {}

        MyRawVisitable(const string &s) :
            data(NULL) {
            data = new char[s.size() + 1];
            sprintf(data, "%s", s.c_str());
            data[s.size()] = 0;
            size = s.size()+1;
        }

        virtual ~MyRawVisitable() {
            delete [] data;
        }
        
        char *data;
        uint32_t size;
        
        virtual void accept(odcore::base::Visitor &v) {
            v.beginVisit(1, "MyNestedVisitable", "MyNestedVisitable");
            v.visit(1, "MyNestedVisitable::data", "data", data, size);
            v.endVisit();
        }

        ostream& operator<<(ostream &out) const {
            SerializationFactory& sf=SerializationFactory::getInstance();

            std::shared_ptr<Serializer> s = sf.getSerializer(out);

            s->write(1,
                    data, 13);

            return out;
        }

        istream& operator>>(istream &in) {
            SerializationFactory& sf=SerializationFactory::getInstance();

            std::shared_ptr<Deserializer> d = sf.getDeserializer(in);

            d->read(1,
                   data, 13);

            return in;
        }
};

class MyNestedVisitable : public odcore::serialization::Serializable, public Visitable {
    public:
        MyNestedVisitable() :
                m_double(0) {}
        
        double m_double;
        
        virtual void accept(odcore::base::Visitor &v) {
            v.beginVisit(1, "MyNestedVisitable", "MyNestedVisitable");
            v.visit(1, "MyNestedVisitable::m_double", "m_double", m_double);
            v.endVisit();
        }

        ostream& operator<<(ostream &out) const {
            SerializationFactory& sf=SerializationFactory::getInstance();

            std::shared_ptr<Serializer> s = sf.getSerializer(out);

            s->write(1, m_double);

            return out;
        }

        istream& operator>>(istream &in) {
            SerializationFactory& sf=SerializationFactory::getInstance();

            std::shared_ptr<Deserializer> d = sf.getDeserializer(in);

            d->read(1, m_double);

            return in;
        }
};

class MyVisitable : public Serializable, public Visitable {
    public:
        MyVisitable() :
            Serializable(),
            Visitable(),
            m_att1(0),
            m_att2(0),
            m_att3(0),
            m_att4(""),
            m_att5() {}

        MyVisitable(const MyVisitable &obj) :
            Serializable(obj),
            Visitable(obj),
            m_att1(obj.m_att1),
            m_att2(obj.m_att2),
            m_att3(obj.m_att3),
            m_att4(obj.m_att4),
            m_att5(obj.m_att5) {}

        ~MyVisitable() {}

        MyVisitable& operator=(const MyVisitable &obj) {
            m_att1 = obj.m_att1;
            m_att2 = obj.m_att2;
            m_att3 = obj.m_att3;
            m_att4 = obj.m_att4;
            m_att5 = obj.m_att5;
            return *this;
        }

        virtual ostream& operator<<(ostream &out) const {
            SerializationFactory& sf=SerializationFactory::getInstance();

            std::shared_ptr<Serializer> s = sf.getSerializer(out);
        
            s->write(1, m_att1);

            s->write(2, m_att2);

            s->write(3, m_att3);

            s->write(4, m_att4);

            s->write(5, m_att5);

            return out;
        }

        virtual istream& operator>>(istream &in) {
            SerializationFactory& sf=SerializationFactory::getInstance();
        
            std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
        
            d->read(1, m_att1);

            d->read(2, m_att2);

            d->read(3, m_att3);

            d->read(4, m_att4);

            d->read(5, m_att5);

            return in;
        }

        virtual void accept(odcore::base::Visitor &v) {
            v.beginVisit(1, "MyVisitable", "MyVisitable");
            v.visit(1, "MyVisitable::att1", "att1", m_att1);
            v.visit(2, "MyVisitable::att2", "att2", m_att2);
            v.visit(3, "MyVisitable::att3", "att3", m_att3);
            v.visit(4, "MyVisitable::att4", "att4", m_att4);
            v.visit(5, "MyVisitable::att5", "att5", m_att5);
            v.endVisit();
        }

    public:
        uint32_t m_att1;
        float m_att2;
        double m_att3;
        string m_att4;
        MyNestedVisitable m_att5;
};

class FieldTest : public CxxTest::TestSuite {
    public:
        void testMessage1() {
            MyNestedVisitable d;
            d.m_double = 1.234;

            // Pretty print the data structure.
            MessagePrettyPrinterVisitor mpp1;
            d.accept(mpp1);
            stringstream sstr1;
            mpp1.getOutput(sstr1);

            // Create generic representation from data structure.
            MessageFromVisitableVisitor mfvv;
            d.accept(mfvv);
            Message msg = mfvv.getMessage();

            // Pretty print generic representation.
            MessagePrettyPrinterVisitor mpp2;
            msg.accept(mpp2);
            stringstream sstr2;
            mpp2.getOutput(sstr2);

            // Assert both pretty printed representations are identical.
            TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(sstr1.str(), sstr2.str()));

            // Restore concrete data structure from generic representation.
            MessageToVisitableVisitor mtvv(msg);
            MyNestedVisitable d2;
            TS_ASSERT(fabs(d2.m_double) < 1e-4);

            d2.accept(mtvv);
            TS_ASSERT(fabs(d2.m_double - d.m_double) < 1e-4);
        }

        void testMessage2() {
            MyVisitable d;
            d.m_att1 = 10;
            d.m_att2 = 1.234;
            d.m_att3 = -4.5789;
            d.m_att4 = "Hello World!";
            d.m_att5.m_double = 1.234;

            // Pretty print the data structure.
            MessagePrettyPrinterVisitor mpp1;
            d.accept(mpp1);
            stringstream sstr1;
            mpp1.getOutput(sstr1);

            // Create generic representation from data structure.
            MessageFromVisitableVisitor mfvv;
            d.accept(mfvv);
            Message msg = mfvv.getMessage();

            // Pretty print generic representation.
            MessagePrettyPrinterVisitor mpp2;
            msg.accept(mpp2);
            stringstream sstr2;
            mpp2.getOutput(sstr2);

            // Assert both pretty printed representations are identical.
            TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(sstr1.str(), sstr2.str()));

            // Restore concrete data structure from generic representation.
            MessageToVisitableVisitor mtvv(msg);
            MyVisitable d2;
            TS_ASSERT(d2.m_att1 == 0);
            TS_ASSERT(fabs(d2.m_att2) < 1e-4);
            TS_ASSERT(fabs(d2.m_att3) < 1e-4);
            TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(d2.m_att4, ""));
            TS_ASSERT(fabs(d2.m_att5.m_double) < 1e-4);

            d2.accept(mtvv);
            TS_ASSERT(d2.m_att1 == d.m_att1);
            TS_ASSERT(fabs(d2.m_att2 - d.m_att2) < 1e-4);
            TS_ASSERT(fabs(d2.m_att3 - d.m_att3) < 1e-4);
            TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(d2.m_att4, d.m_att4));
            TS_ASSERT(fabs(d2.m_att5.m_double - d.m_att5.m_double) < 1e-4);
        }

        void testMessage3() {
            MyRawVisitable d("Hello World!");

            // Pretty print the data structure.
            MessagePrettyPrinterVisitor mpp1;
            d.accept(mpp1);
            stringstream sstr1;
            mpp1.getOutput(sstr1);

            // Create generic representation from data structure.
            MessageFromVisitableVisitor mfvv;
            d.accept(mfvv);
            Message msg = mfvv.getMessage();

            // Pretty print generic representation.
            MessagePrettyPrinterVisitor mpp2;
            msg.accept(mpp2);
            stringstream sstr2;
            mpp2.getOutput(sstr2);

            // Assert both pretty printed representations are identical.
            TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(sstr1.str(), sstr2.str()));

            // Restore concrete data structure from generic representation.
            MessageToVisitableVisitor mtvv(msg);
            MyRawVisitable d2;
            TS_ASSERT(d2.data == NULL);
            d2.data = static_cast<char*>(calloc(d.size, sizeof(char)));
            d2.size = d.size;

            d2.accept(mtvv);
            TS_ASSERT(strcmp(d.data, d2.data) == 0);
        }

        void testMessageExtractField() {
            // Create a visitable data structure.
            MyVisitable d;
            d.m_att1 = 10;
            d.m_att2 = 1.234;
            d.m_att3 = -4.5789;
            d.m_att4 = "Hello World!";
            d.m_att5.m_double = 1.234;

            // Create generic representation from our data structure.
            MessageFromVisitableVisitor mfvv;
            d.accept(mfvv);
            Message msg = mfvv.getMessage();

            double value = 0;
            bool found = false;
            bool extracted = false;

            found = false; extracted = false; value = msg.getValueFromScalarField<double>(1, found, extracted);
            TS_ASSERT(found); TS_ASSERT(extracted); TS_ASSERT_DELTA(value, 10, 1e-5);

            found = false; extracted = false; value = msg.getValueFromScalarField<double>(2, found, extracted);
            TS_ASSERT(found); TS_ASSERT(extracted); TS_ASSERT_DELTA(value, 1.234, 1e-5);

            found = false; extracted = false; value = msg.getValueFromScalarField<double>(3, found, extracted);
            TS_ASSERT(found); TS_ASSERT(extracted); TS_ASSERT_DELTA(value, -4.5789, 1e-5);

            found = false; extracted = true; value = msg.getValueFromScalarField<double>(4, found, extracted);
            TS_ASSERT(found); TS_ASSERT(!extracted); TS_ASSERT_DELTA(value, 0, 1e-5);

            found = true; extracted = true; value = msg.getValueFromScalarField<double>(6, found, extracted);
            TS_ASSERT(!found); TS_ASSERT(!extracted);
        }

        void testCreateMessageFromVisitableWithFixedArrays() {
#ifdef __arm__
            TS_ASSERT(1 == 1);
#else
            TestMessage10 tm1;
            TS_ASSERT(tm1.getSize_MyArray1() == 2);
            TS_ASSERT(tm1.getSize_MyArray2() == 3);

            uint32_t *tm1_arr1 = tm1.getMyArray1();
            tm1_arr1[0] = 1; tm1_arr1[1] = 2;
            float *tm1_arr2 = tm1.getMyArray2();
            tm1_arr2[0] = -1.2345; tm1_arr2[1] = -2.3456; tm1_arr2[2] = -3.4567;

            TS_ASSERT(tm1.getSize_MyArray1() == 2);
            TS_ASSERT(tm1.getSize_MyArray2() == 3);
            TS_ASSERT(tm1_arr1[0] == 1);
            TS_ASSERT(tm1_arr1[1] == 2);
            TS_ASSERT_DELTA(tm1_arr2[0], -1.2345, 1e-4);
            TS_ASSERT_DELTA(tm1_arr2[1], -2.3456, 1e-4);
            TS_ASSERT_DELTA(tm1_arr2[2], -3.4567, 1e-4);

            TS_ASSERT(tm1.getMyArray1()[0] == 1);
            TS_ASSERT(tm1.getMyArray1()[1] == 2);
            TS_ASSERT_DELTA(tm1.getMyArray2()[0], -1.2345, 1e-4);
            TS_ASSERT_DELTA(tm1.getMyArray2()[1], -2.3456, 1e-4);
            TS_ASSERT_DELTA(tm1.getMyArray2()[2], -3.4567, 1e-4);

            // Create generic representation from our data structure.
            MessageFromVisitableVisitor mfvv;
            tm1.accept(mfvv);
            Message msg = mfvv.getMessage();

            TS_ASSERT(msg.getNumberOfFields() == 2);

            bool found = false;
            std::shared_ptr<odcore::data::reflection::AbstractField> af1 = msg.getFieldByIdentifier(1, found);
            TS_ASSERT(found == true);
            TS_ASSERT(af1->getFieldIdentifier() == 1);
            TS_ASSERT(af1->getLongFieldName() == "TestMessage10.myArray1");
            TS_ASSERT(af1->getShortFieldName() == "myArray1");

            std::shared_ptr<char> valueF1 = dynamic_cast<Field<std::shared_ptr<char> >*>(af1.get())->getValue();
            char *valueF1Ptr = valueF1.operator->();
            uint32_t *f1Ptr = reinterpret_cast<uint32_t*>(valueF1Ptr);
            TS_ASSERT(f1Ptr[0] == 1);
            TS_ASSERT(f1Ptr[1] == 2);
            TS_ASSERT(af1->getSize() == sizeof(uint32_t));
            TS_ASSERT(af1->getIsFixedArray() == true);
            TS_ASSERT(af1->getNumberOfElementsInFixedArray() == 2);

            found = false;
            std::shared_ptr<odcore::data::reflection::AbstractField> af2 = msg.getFieldByIdentifier(2, found);
            TS_ASSERT(found == true);
            TS_ASSERT(af2->getFieldIdentifier() == 2);
            TS_ASSERT(af2->getLongFieldName() == "TestMessage10.myArray2");
            TS_ASSERT(af2->getShortFieldName() == "myArray2");
            std::shared_ptr<char> valueF2 = dynamic_cast<Field<std::shared_ptr<char> >*>(af2.get())->getValue();
            char *valueF2Ptr = valueF2.operator->();
            float *f2Ptr = reinterpret_cast<float*>(valueF2Ptr);
            TS_ASSERT_DELTA(f2Ptr[0], -1.2345, 1e-4);
            TS_ASSERT_DELTA(f2Ptr[1], -2.3456, 1e-4);
            TS_ASSERT_DELTA(f2Ptr[2], -3.4567, 1e-4);
            TS_ASSERT(af2->getSize() == sizeof(float));
            TS_ASSERT(af2->getIsFixedArray() == true);
            TS_ASSERT(af2->getNumberOfElementsInFixedArray() == 3);

            found = false;
            msg.getFieldByIdentifier(3, found);
            TS_ASSERT(found == false);

            // Apply CSV exporter to generic message.
            stringstream output;
            const bool ADD_HEADER = true;
            const char DELIMITER = '%';

            stringstream expected;
            expected << "myArray1%myArray2%" << endl;
            expected << "(1,2)%(-1.2345,-2.3456,-3.4567)%" << endl;

            CSVFromVisitableVisitor csv(output, ADD_HEADER, DELIMITER);
            msg.accept(csv);

            TS_ASSERT(output.str() == expected.str());
#endif
        }
};

#endif /*CORE_MESSAGETESTSUITE_H_*/
