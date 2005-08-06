/*  cds_resource.cc - this file is part of MediaTomb.
                                                                                
    Copyright (C) 2005 Gena Batyan <bgeradz@deadlock.dhs.org>,
                       Sergey Bostandzhyan <jin@deadlock.dhs.org>
                                                                                
    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
                                                                                
    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
                                                                                
    You should have received a copy of the GNU General Public License
    along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "tools.h"
#include "cds_resource.h"

using namespace zmm;

CdsResource::CdsResource(int handler_type) : Object()
{
    this->handlerType = handlerType;
    this->attributes = Ref<Dictionary>(new Dictionary());
    this->parameters = Ref<Dictionary>(new Dictionary());
}
CdsResource::CdsResource(int handlerType,
                         Ref<Dictionary> attributes,
                         Ref<Dictionary> parameters)
{
    this->handlerType = handlerType;
    this->attributes = attributes;
    this->parameters = parameters;
}

void CdsResource::addAttribute(zmm::String name, zmm::String value)
{
    attributes->put(name, value);
}
void CdsResource::addParameter(zmm::String name, zmm::String value)
{
    parameters->put(name, value);
}

int CdsResource::getHandlerType() 
{
    return handlerType;
}
Ref<Dictionary> CdsResource::getAttributes()
{
    return attributes;
}
Ref<Dictionary> CdsResource::getParameters()
{
    return parameters;
}

bool CdsResource::equals(Ref<CdsResource> other)
{
    return (
        handlerType == other->handlerType &&
        attributes->equals(other->attributes) &&
        parameters->equals(other->parameters)
    );
}

String CdsResource::encode()
{
    // encode resources
    Ref<StringBuffer> buf(new StringBuffer());
    *buf << handlerType;
    *buf << RESOURCE_PART_SEP;
    *buf << attributes->encode();
    *buf << RESOURCE_PART_SEP;
    *buf << parameters->encode();
    return buf->toString();
}

Ref<CdsResource> CdsResource::decode(String serial)
{
    Ref<Array<StringBase> > parts = split_string(serial, RESOURCE_PART_SEP);
    int size = parts->size();
    if (size != 2 && size != 3)
        throw Exception("CdsResource::decode: Could not parse resources");
    int handlerType = String(parts->get(0)).toInt();
    
    Ref<Dictionary> attr(new Dictionary());
    attr->decode(parts->get(1));

    Ref<Dictionary> par(new Dictionary());

    if (size == 3)
        par->decode(parts->get(2));
    
    Ref<CdsResource> resource(new CdsResource(handlerType, attr, par));
    
    return resource;
}

