//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef __INET_IPV4HEADERSERIALIZER_H
#define __INET_IPV4HEADERSERIALIZER_H

#include "inet/common/packet/serializer/FieldsChunkSerializer.h"
#include "inet/networklayer/ipv4/Ipv4Header.h"

namespace inet {

namespace serializer {

/**
 * Converts between Ipv4Header and binary (network byte order) IPv4 header.
 */
class INET_API Ipv4HeaderSerializer : public FieldsChunkSerializer
{
  protected:
    virtual void serializeOption(MemoryOutputStream& stream, const TlvOptionBase *option) const;
    virtual TlvOptionBase *deserializeOption(MemoryInputStream& stream) const;

    virtual void serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const override;
    virtual const Ptr<Chunk> deserialize(MemoryInputStream& stream) const override;

  public:
    Ipv4HeaderSerializer() : FieldsChunkSerializer() {}
};

} // namespace serializer

} // namespace inet

#endif // ifndef __INET_IPV4HEADERSERIALIZER_H
