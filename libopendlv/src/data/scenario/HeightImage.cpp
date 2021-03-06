/**
 * OpenDLV - Simulation environment
 * Copyright (C) 2008 - 2015 Christian Berger, Bernhard Rumpe
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <memory>
#include <ostream>
#include <string>

#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"
#include "opendlv/data/scenario/HeightImage.h"
#include "opendlv/data/scenario/Image.h"
#include "opendlv/data/scenario/ScenarioVisitor.h"

namespace opendlv {
    namespace data {
        namespace scenario {

            using namespace std;
            using namespace odcore::base;

            HeightImage::HeightImage() :
                    Image(),
                    m_groundHeight(0),
                    m_minimumHeight(0),
                    m_maximumHeight(0) {}

            HeightImage::HeightImage(const HeightImage &obj) :
                    Image(obj),
                    m_groundHeight(obj.m_groundHeight),
                    m_minimumHeight(obj.m_minimumHeight),
                    m_maximumHeight(obj.m_maximumHeight) {}

            HeightImage::~HeightImage() {}

            HeightImage& HeightImage::operator=(const HeightImage &obj) {
                Image::operator=(obj);
                setGroundHeight(obj.getGroundHeight());
                setMinimumHeight(obj.getMinimumHeight());
                setMaximumHeight(obj.getMaximumHeight());
                return (*this);
            }

            void HeightImage::accept(ScenarioVisitor &visitor) {
                Image::accept(visitor);
                visitor.visit(*this);
            }

            double HeightImage::getGroundHeight() const {
                return m_groundHeight;
            }

            void HeightImage::setGroundHeight(const double &gh) {
                m_groundHeight = gh;
            }

            double HeightImage::getMinimumHeight() const {
                return m_minimumHeight;
            }

            void HeightImage::setMinimumHeight(const double &minH) {
                m_minimumHeight = minH;
            }

            double HeightImage::getMaximumHeight() const {
                return m_maximumHeight;
            }

            void HeightImage::setMaximumHeight(const double &maxH) {
                m_maximumHeight = maxH;
            }

            int32_t HeightImage::getID() const {
                return 819;
            }

            const string HeightImage::getShortName() const {
                return "HeightImage";
            }

            const string HeightImage::getLongName() const {
                return "hesperia.data.scenario.HeightImage";
            }

            const string HeightImage::toString() const {
                stringstream s;
                s << Image::toString()
                << ", GroundHeight: " << m_groundHeight << ", MinimumHeight: " << m_minimumHeight << ", MaximumHeight: " << m_maximumHeight;
                return s.str();
            }

            ostream& HeightImage::operator<<(ostream &out) const {
                // Serializer super class.
                Image::operator<<(out);

                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();

                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);

                s->write(1, m_groundHeight);

                s->write(2, m_minimumHeight);

                s->write(3, m_maximumHeight);

                return out;
            }

            istream& HeightImage::operator>>(istream &in) {
                // Deserializer super class.
                Image::operator>>(in);

                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();

                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);

                d->read(1, m_groundHeight);

                d->read(2, m_minimumHeight);

                d->read(3, m_maximumHeight);

                return in;
            }

        }
    }
} // opendlv::data::scenario
