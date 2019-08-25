// Copyright (c) 2005-2017 Code Synthesis Tools CC
//
// This program was generated by CodeSynthesis XSD, an XML Schema to
// C++ data binding compiler.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
// In addition, as a special exception, Code Synthesis Tools CC gives
// permission to link this program with the Xerces-C++ library (or with
// modified versions of Xerces-C++ that use the same license as Xerces-C++),
// and distribute linked combinations including the two. You must obey
// the GNU General Public License version 2 in all respects for all of
// the code used other than Xerces-C++. If you modify this copy of the
// program, you may extend this exception to your version of the program,
// but you are not obligated to do so. If you do not wish to do so, delete
// this exception statement from your version.
//
// Furthermore, Code Synthesis Tools CC makes a special exception for
// the Free/Libre and Open Source Software (FLOSS) which is described
// in the accompanying FLOSSE file.
//

// Begin prologue.
//
//
// End prologue.

#include <xsd/cxx/pre.hxx>

#include "Structure.hxx"

#include <xsd/cxx/xml/dom/parsing-source.hxx>

// SimpleType
//

SimpleType::
SimpleType (const ::xercesc::DOMElement& e,
            ::xml_schema::Flags f,
            ::xml_schema::Container* c)
: ::xml_schema::String (e, f, c)
{
  _xsd_SimpleType_convert ();
}

SimpleType::
SimpleType (const ::xercesc::DOMAttr& a,
            ::xml_schema::Flags f,
            ::xml_schema::Container* c)
: ::xml_schema::String (a, f, c)
{
  _xsd_SimpleType_convert ();
}

SimpleType::
SimpleType (const ::std::string& s,
            const ::xercesc::DOMElement* e,
            ::xml_schema::Flags f,
            ::xml_schema::Container* c)
: ::xml_schema::String (s, e, f, c)
{
  _xsd_SimpleType_convert ();
}

SimpleType* SimpleType::
_clone (::xml_schema::Flags f,
        ::xml_schema::Container* c) const
{
  return new class SimpleType (*this, f, c);
}

SimpleType::Value SimpleType::
_xsd_SimpleType_convert () const
{
  ::xsd::cxx::tree::enum_comparator< char > c (_xsd_SimpleType_literals_);
  const Value* i (::std::lower_bound (
                    _xsd_SimpleType_indexes_,
                    _xsd_SimpleType_indexes_ + 10,
                    *this,
                    c));

  if (i == _xsd_SimpleType_indexes_ + 10 || _xsd_SimpleType_literals_[*i] != *this)
  {
    throw ::xsd::cxx::tree::unexpected_enumerator < char > (*this);
  }

  return *i;
}

const char* const SimpleType::
_xsd_SimpleType_literals_[10] =
{
  "unsigned_int_8",
  "unsigned_int_16",
  "unsigned_int_32",
  "unsigned_int_64",
  "int_8",
  "int_16",
  "int_32",
  "int_64",
  "float_32",
  "float_64"
};

const SimpleType::Value SimpleType::
_xsd_SimpleType_indexes_[10] =
{
  ::SimpleType::float_32,
  ::SimpleType::float_64,
  ::SimpleType::int_16,
  ::SimpleType::int_32,
  ::SimpleType::int_64,
  ::SimpleType::int_8,
  ::SimpleType::unsigned_int_16,
  ::SimpleType::unsigned_int_32,
  ::SimpleType::unsigned_int_64,
  ::SimpleType::unsigned_int_8
};

// ExtendedType
//

ExtendedType::
ExtendedType (const ::xercesc::DOMElement& e,
              ::xml_schema::Flags f,
              ::xml_schema::Container* c)
: ::xml_schema::String (e, f, c)
{
  _xsd_ExtendedType_convert ();
}

ExtendedType::
ExtendedType (const ::xercesc::DOMAttr& a,
              ::xml_schema::Flags f,
              ::xml_schema::Container* c)
: ::xml_schema::String (a, f, c)
{
  _xsd_ExtendedType_convert ();
}

ExtendedType::
ExtendedType (const ::std::string& s,
              const ::xercesc::DOMElement* e,
              ::xml_schema::Flags f,
              ::xml_schema::Container* c)
: ::xml_schema::String (s, e, f, c)
{
  _xsd_ExtendedType_convert ();
}

ExtendedType* ExtendedType::
_clone (::xml_schema::Flags f,
        ::xml_schema::Container* c) const
{
  return new class ExtendedType (*this, f, c);
}

ExtendedType::Value ExtendedType::
_xsd_ExtendedType_convert () const
{
  ::xsd::cxx::tree::enum_comparator< char > c (_xsd_ExtendedType_literals_);
  const Value* i (::std::lower_bound (
                    _xsd_ExtendedType_indexes_,
                    _xsd_ExtendedType_indexes_ + 15,
                    *this,
                    c));

  if (i == _xsd_ExtendedType_indexes_ + 15 || _xsd_ExtendedType_literals_[*i] != *this)
  {
    throw ::xsd::cxx::tree::unexpected_enumerator < char > (*this);
  }

  return *i;
}

const char* const ExtendedType::
_xsd_ExtendedType_literals_[15] =
{
  "string",
  "complex_unsigned_int_8",
  "complex_unsigned_int_16",
  "complex_unsigned_int_32",
  "complex_unsigned_int_64",
  "complex_int_8",
  "complex_int_16",
  "complex_int_32",
  "complex_int_64",
  "complex_float_32",
  "complex_float_64",
  "array",
  "sequence",
  "map",
  "structure"
};

const ExtendedType::Value ExtendedType::
_xsd_ExtendedType_indexes_[15] =
{
  ::ExtendedType::array,
  ::ExtendedType::complex_float_32,
  ::ExtendedType::complex_float_64,
  ::ExtendedType::complex_int_16,
  ::ExtendedType::complex_int_32,
  ::ExtendedType::complex_int_64,
  ::ExtendedType::complex_int_8,
  ::ExtendedType::complex_unsigned_int_16,
  ::ExtendedType::complex_unsigned_int_32,
  ::ExtendedType::complex_unsigned_int_64,
  ::ExtendedType::complex_unsigned_int_8,
  ::ExtendedType::map,
  ::ExtendedType::sequence,
  ::ExtendedType::string,
  ::ExtendedType::structure
};

// Type
//

Type::
Type (const ::xercesc::DOMElement& e,
      ::xml_schema::Flags f,
      ::xml_schema::Container* c)
: ::xml_schema::String (e, f, c)
{
  _xsd_Type_convert ();
}

Type::
Type (const ::xercesc::DOMAttr& a,
      ::xml_schema::Flags f,
      ::xml_schema::Container* c)
: ::xml_schema::String (a, f, c)
{
  _xsd_Type_convert ();
}

Type::
Type (const ::std::string& s,
      const ::xercesc::DOMElement* e,
      ::xml_schema::Flags f,
      ::xml_schema::Container* c)
: ::xml_schema::String (s, e, f, c)
{
  _xsd_Type_convert ();
}

Type* Type::
_clone (::xml_schema::Flags f,
        ::xml_schema::Container* c) const
{
  return new class Type (*this, f, c);
}

Type::Value Type::
_xsd_Type_convert () const
{
  ::xsd::cxx::tree::enum_comparator< char > c (_xsd_Type_literals_);
  const Value* i (::std::lower_bound (
                    _xsd_Type_indexes_,
                    _xsd_Type_indexes_ + 25,
                    *this,
                    c));

  if (i == _xsd_Type_indexes_ + 25 || _xsd_Type_literals_[*i] != *this)
  {
    throw ::xsd::cxx::tree::unexpected_enumerator < char > (*this);
  }

  return *i;
}

const char* const Type::
_xsd_Type_literals_[25] =
{
  "unsigned_int_8",
  "unsigned_int_16",
  "unsigned_int_32",
  "unsigned_int_64",
  "int_8",
  "int_16",
  "int_32",
  "int_64",
  "float_32",
  "float_64",
  "string",
  "complex_unsigned_int_8",
  "complex_unsigned_int_16",
  "complex_unsigned_int_32",
  "complex_unsigned_int_64",
  "complex_int_8",
  "complex_int_16",
  "complex_int_32",
  "complex_int_64",
  "complex_float_32",
  "complex_float_64",
  "array",
  "sequence",
  "map",
  "structure"
};

const Type::Value Type::
_xsd_Type_indexes_[25] =
{
  ::Type::array,
  ::Type::complex_float_32,
  ::Type::complex_float_64,
  ::Type::complex_int_16,
  ::Type::complex_int_32,
  ::Type::complex_int_64,
  ::Type::complex_int_8,
  ::Type::complex_unsigned_int_16,
  ::Type::complex_unsigned_int_32,
  ::Type::complex_unsigned_int_64,
  ::Type::complex_unsigned_int_8,
  ::Type::float_32,
  ::Type::float_64,
  ::Type::int_16,
  ::Type::int_32,
  ::Type::int_64,
  ::Type::int_8,
  ::Type::map,
  ::Type::sequence,
  ::Type::string,
  ::Type::structure,
  ::Type::unsigned_int_16,
  ::Type::unsigned_int_32,
  ::Type::unsigned_int_64,
  ::Type::unsigned_int_8
};

// Constant
//

Constant::
Constant (const NameType& name,
          const TypeType& type,
          const ValueType& value)
: ::xml_schema::Type (),
  documentation_ (this),
  name_ (name, this),
  type_ (type, this),
  value_ (value, this)
{
}

Constant::
Constant (const Constant& x,
          ::xml_schema::Flags f,
          ::xml_schema::Container* c)
: ::xml_schema::Type (x, f, c),
  documentation_ (x.documentation_, f, this),
  name_ (x.name_, f, this),
  type_ (x.type_, f, this),
  value_ (x.value_, f, this)
{
}

Constant::
Constant (const ::xercesc::DOMElement& e,
          ::xml_schema::Flags f,
          ::xml_schema::Container* c)
: ::xml_schema::Type (e, f | ::xml_schema::Flags::base, c),
  documentation_ (this),
  name_ (this),
  type_ (this),
  value_ (this)
{
  if ((f & ::xml_schema::Flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false, true);
    this->parse (p, f);
  }
}

void Constant::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::Flags f)
{
  for (; p.more_content (); p.next_content (false))
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // documentation
    //
    if (n.name () == "documentation" && n.namespace_ ().empty ())
    {
      ::std::unique_ptr< DocumentationType > r (
        DocumentationTraits::create (i, f, this));

      if (!this->documentation_)
      {
        this->documentation_.set (::std::move (r));
        continue;
      }
    }

    break;
  }

  while (p.more_attributes ())
  {
    const ::xercesc::DOMAttr& i (p.next_attribute ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    if (n.name () == "name" && n.namespace_ ().empty ())
    {
      this->name_.set (NameTraits::create (i, f, this));
      continue;
    }

    if (n.name () == "type" && n.namespace_ ().empty ())
    {
      this->type_.set (TypeTraits::create (i, f, this));
      continue;
    }

    if (n.name () == "value" && n.namespace_ ().empty ())
    {
      this->value_.set (ValueTraits::create (i, f, this));
      continue;
    }
  }

  if (!name_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "name",
      "");
  }

  if (!type_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "type",
      "");
  }

  if (!value_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "value",
      "");
  }
}

Constant* Constant::
_clone (::xml_schema::Flags f,
        ::xml_schema::Container* c) const
{
  return new class Constant (*this, f, c);
}

Constant& Constant::
operator= (const Constant& x)
{
  if (this != &x)
  {
    static_cast< ::xml_schema::Type& > (*this) = x;
    this->documentation_ = x.documentation_;
    this->name_ = x.name_;
    this->type_ = x.type_;
    this->value_ = x.value_;
  }

  return *this;
}

Constant::
~Constant ()
{
}

// Element
//

Element::
Element (const TypeType& type)
: ::xml_schema::Type (),
  documentation_ (this),
  subElement_ (this),
  subElement2_ (this),
  name_ (this),
  arraySize_ (this),
  type_ (type, this),
  structureName_ (this)
{
}

Element::
Element (const Element& x,
         ::xml_schema::Flags f,
         ::xml_schema::Container* c)
: ::xml_schema::Type (x, f, c),
  documentation_ (x.documentation_, f, this),
  subElement_ (x.subElement_, f, this),
  subElement2_ (x.subElement2_, f, this),
  name_ (x.name_, f, this),
  arraySize_ (x.arraySize_, f, this),
  type_ (x.type_, f, this),
  structureName_ (x.structureName_, f, this)
{
}

Element::
Element (const ::xercesc::DOMElement& e,
         ::xml_schema::Flags f,
         ::xml_schema::Container* c)
: ::xml_schema::Type (e, f | ::xml_schema::Flags::base, c),
  documentation_ (this),
  subElement_ (this),
  subElement2_ (this),
  name_ (this),
  arraySize_ (this),
  type_ (this),
  structureName_ (this)
{
  if ((f & ::xml_schema::Flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false, true);
    this->parse (p, f);
  }
}

void Element::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::Flags f)
{
  for (; p.more_content (); p.next_content (false))
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // documentation
    //
    if (n.name () == "documentation" && n.namespace_ ().empty ())
    {
      ::std::unique_ptr< DocumentationType > r (
        DocumentationTraits::create (i, f, this));

      if (!this->documentation_)
      {
        this->documentation_.set (::std::move (r));
        continue;
      }
    }

    // subElement
    //
    if (n.name () == "subElement" && n.namespace_ ().empty ())
    {
      ::std::unique_ptr< SubElementType > r (
        SubElementTraits::create (i, f, this));

      if (!this->subElement_)
      {
        this->subElement_.set (::std::move (r));
        continue;
      }
    }

    // subElement2
    //
    if (n.name () == "subElement2" && n.namespace_ ().empty ())
    {
      ::std::unique_ptr< SubElement2Type > r (
        SubElement2Traits::create (i, f, this));

      if (!this->subElement2_)
      {
        this->subElement2_.set (::std::move (r));
        continue;
      }
    }

    break;
  }

  while (p.more_attributes ())
  {
    const ::xercesc::DOMAttr& i (p.next_attribute ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    if (n.name () == "name" && n.namespace_ ().empty ())
    {
      this->name_.set (NameTraits::create (i, f, this));
      continue;
    }

    if (n.name () == "arraySize" && n.namespace_ ().empty ())
    {
      this->arraySize_.set (ArraySizeTraits::create (i, f, this));
      continue;
    }

    if (n.name () == "type" && n.namespace_ ().empty ())
    {
      this->type_.set (TypeTraits::create (i, f, this));
      continue;
    }

    if (n.name () == "structureName" && n.namespace_ ().empty ())
    {
      this->structureName_.set (StructureNameTraits::create (i, f, this));
      continue;
    }
  }

  if (!type_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "type",
      "");
  }
}

Element* Element::
_clone (::xml_schema::Flags f,
        ::xml_schema::Container* c) const
{
  return new class Element (*this, f, c);
}

Element& Element::
operator= (const Element& x)
{
  if (this != &x)
  {
    static_cast< ::xml_schema::Type& > (*this) = x;
    this->documentation_ = x.documentation_;
    this->subElement_ = x.subElement_;
    this->subElement2_ = x.subElement2_;
    this->name_ = x.name_;
    this->arraySize_ = x.arraySize_;
    this->type_ = x.type_;
    this->structureName_ = x.structureName_;
  }

  return *this;
}

Element::
~Element ()
{
}

// Constants
//

Constants::
Constants ()
: ::xml_schema::Type (),
  constant_ (this)
{
}

Constants::
Constants (const Constants& x,
           ::xml_schema::Flags f,
           ::xml_schema::Container* c)
: ::xml_schema::Type (x, f, c),
  constant_ (x.constant_, f, this)
{
}

Constants::
Constants (const ::xercesc::DOMElement& e,
           ::xml_schema::Flags f,
           ::xml_schema::Container* c)
: ::xml_schema::Type (e, f | ::xml_schema::Flags::base, c),
  constant_ (this)
{
  if ((f & ::xml_schema::Flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false, false);
    this->parse (p, f);
  }
}

void Constants::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::Flags f)
{
  for (; p.more_content (); p.next_content (false))
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // constant
    //
    if (n.name () == "constant" && n.namespace_ ().empty ())
    {
      ::std::unique_ptr< ConstantType > r (
        ConstantTraits::create (i, f, this));

      this->constant_.push_back (::std::move (r));
      continue;
    }

    break;
  }
}

Constants* Constants::
_clone (::xml_schema::Flags f,
        ::xml_schema::Container* c) const
{
  return new class Constants (*this, f, c);
}

Constants& Constants::
operator= (const Constants& x)
{
  if (this != &x)
  {
    static_cast< ::xml_schema::Type& > (*this) = x;
    this->constant_ = x.constant_;
  }

  return *this;
}

Constants::
~Constants ()
{
}

// Elements
//

Elements::
Elements ()
: ::xml_schema::Type (),
  element_ (this)
{
}

Elements::
Elements (const Elements& x,
          ::xml_schema::Flags f,
          ::xml_schema::Container* c)
: ::xml_schema::Type (x, f, c),
  element_ (x.element_, f, this)
{
}

Elements::
Elements (const ::xercesc::DOMElement& e,
          ::xml_schema::Flags f,
          ::xml_schema::Container* c)
: ::xml_schema::Type (e, f | ::xml_schema::Flags::base, c),
  element_ (this)
{
  if ((f & ::xml_schema::Flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false, false);
    this->parse (p, f);
  }
}

void Elements::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::Flags f)
{
  for (; p.more_content (); p.next_content (false))
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // element
    //
    if (n.name () == "element" && n.namespace_ ().empty ())
    {
      ::std::unique_ptr< ElementType > r (
        ElementTraits::create (i, f, this));

      this->element_.push_back (::std::move (r));
      continue;
    }

    break;
  }
}

Elements* Elements::
_clone (::xml_schema::Flags f,
        ::xml_schema::Container* c) const
{
  return new class Elements (*this, f, c);
}

Elements& Elements::
operator= (const Elements& x)
{
  if (this != &x)
  {
    static_cast< ::xml_schema::Type& > (*this) = x;
    this->element_ = x.element_;
  }

  return *this;
}

Elements::
~Elements ()
{
}

// Structure
//

Structure::
Structure (const NameType& name)
: ::xml_schema::Type (),
  documentation_ (this),
  constants_ (this),
  elements_ (this),
  name_ (name, this)
{
}

Structure::
Structure (const Structure& x,
           ::xml_schema::Flags f,
           ::xml_schema::Container* c)
: ::xml_schema::Type (x, f, c),
  documentation_ (x.documentation_, f, this),
  constants_ (x.constants_, f, this),
  elements_ (x.elements_, f, this),
  name_ (x.name_, f, this)
{
}

Structure::
Structure (const ::xercesc::DOMElement& e,
           ::xml_schema::Flags f,
           ::xml_schema::Container* c)
: ::xml_schema::Type (e, f | ::xml_schema::Flags::base, c),
  documentation_ (this),
  constants_ (this),
  elements_ (this),
  name_ (this)
{
  if ((f & ::xml_schema::Flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false, true);
    this->parse (p, f);
  }
}

void Structure::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::Flags f)
{
  for (; p.more_content (); p.next_content (false))
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // documentation
    //
    if (n.name () == "documentation" && n.namespace_ ().empty ())
    {
      ::std::unique_ptr< DocumentationType > r (
        DocumentationTraits::create (i, f, this));

      if (!this->documentation_)
      {
        this->documentation_.set (::std::move (r));
        continue;
      }
    }

    // constants
    //
    if (n.name () == "constants" && n.namespace_ ().empty ())
    {
      ::std::unique_ptr< ConstantsType > r (
        ConstantsTraits::create (i, f, this));

      if (!this->constants_)
      {
        this->constants_.set (::std::move (r));
        continue;
      }
    }

    // elements
    //
    if (n.name () == "elements" && n.namespace_ ().empty ())
    {
      ::std::unique_ptr< ElementsType > r (
        ElementsTraits::create (i, f, this));

      if (!this->elements_)
      {
        this->elements_.set (::std::move (r));
        continue;
      }
    }

    break;
  }

  while (p.more_attributes ())
  {
    const ::xercesc::DOMAttr& i (p.next_attribute ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    if (n.name () == "name" && n.namespace_ ().empty ())
    {
      this->name_.set (NameTraits::create (i, f, this));
      continue;
    }
  }

  if (!name_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "name",
      "");
  }
}

Structure* Structure::
_clone (::xml_schema::Flags f,
        ::xml_schema::Container* c) const
{
  return new class Structure (*this, f, c);
}

Structure& Structure::
operator= (const Structure& x)
{
  if (this != &x)
  {
    static_cast< ::xml_schema::Type& > (*this) = x;
    this->documentation_ = x.documentation_;
    this->constants_ = x.constants_;
    this->elements_ = x.elements_;
    this->name_ = x.name_;
  }

  return *this;
}

Structure::
~Structure ()
{
}

#include <ostream>

::std::ostream&
operator<< (::std::ostream& o, SimpleType::Value i)
{
  return o << SimpleType::_xsd_SimpleType_literals_[i];
}

::std::ostream&
operator<< (::std::ostream& o, const SimpleType& i)
{
  return o << static_cast< const ::xml_schema::String& > (i);
}

::std::ostream&
operator<< (::std::ostream& o, ExtendedType::Value i)
{
  return o << ExtendedType::_xsd_ExtendedType_literals_[i];
}

::std::ostream&
operator<< (::std::ostream& o, const ExtendedType& i)
{
  return o << static_cast< const ::xml_schema::String& > (i);
}

::std::ostream&
operator<< (::std::ostream& o, Type::Value i)
{
  return o << Type::_xsd_Type_literals_[i];
}

::std::ostream&
operator<< (::std::ostream& o, const Type& i)
{
  return o << static_cast< const ::xml_schema::String& > (i);
}

::std::ostream&
operator<< (::std::ostream& o, const Constant& i)
{
  if (i.getDocumentation ())
  {
    o << ::std::endl << "documentation: " << *i.getDocumentation ();
  }

  o << ::std::endl << "name: " << i.getName ();
  o << ::std::endl << "type: " << i.getType ();
  o << ::std::endl << "value: " << i.getValue ();
  return o;
}

::std::ostream&
operator<< (::std::ostream& o, const Element& i)
{
  if (i.getDocumentation ())
  {
    o << ::std::endl << "documentation: " << *i.getDocumentation ();
  }

  if (i.getSubElement ())
  {
    o << ::std::endl << "subElement: " << *i.getSubElement ();
  }

  if (i.getSubElement2 ())
  {
    o << ::std::endl << "subElement2: " << *i.getSubElement2 ();
  }

  if (i.getName ())
  {
    o << ::std::endl << "name: " << *i.getName ();
  }

  if (i.getArraySize ())
  {
    o << ::std::endl << "arraySize: " << *i.getArraySize ();
  }

  o << ::std::endl << "type: " << i.getType ();
  if (i.getStructureName ())
  {
    o << ::std::endl << "structureName: " << *i.getStructureName ();
  }

  return o;
}

::std::ostream&
operator<< (::std::ostream& o, const Constants& i)
{
  for (Constants::ConstantConstIterator
       b (i.getConstant ().begin ()), e (i.getConstant ().end ());
       b != e; ++b)
  {
    o << ::std::endl << "constant: " << *b;
  }

  return o;
}

::std::ostream&
operator<< (::std::ostream& o, const Elements& i)
{
  for (Elements::ElementConstIterator
       b (i.getElement ().begin ()), e (i.getElement ().end ());
       b != e; ++b)
  {
    o << ::std::endl << "element: " << *b;
  }

  return o;
}

::std::ostream&
operator<< (::std::ostream& o, const Structure& i)
{
  if (i.getDocumentation ())
  {
    o << ::std::endl << "documentation: " << *i.getDocumentation ();
  }

  if (i.getConstants ())
  {
    o << ::std::endl << "constants: " << *i.getConstants ();
  }

  if (i.getElements ())
  {
    o << ::std::endl << "elements: " << *i.getElements ();
  }

  o << ::std::endl << "name: " << i.getName ();
  return o;
}

#include <istream>
#include <xsd/cxx/xml/sax/std-input-source.hxx>
#include <xsd/cxx/tree/error-handler.hxx>

::std::unique_ptr< ::Structure >
parseStructure (const ::std::string& u,
                ::xml_schema::Flags f,
                const ::xml_schema::Properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::Flags::dont_initialize) == 0,
    (f & ::xml_schema::Flags::keep_dom) == 0);

  ::xsd::cxx::tree::error_handler< char > h;

  ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      u, h, p, f));

  h.throw_if_failed< ::xsd::cxx::tree::parsing< char > > ();

  return ::std::unique_ptr< ::Structure > (
    ::parseStructure (
      std::move (d), f | ::xml_schema::Flags::own_dom, p));
}

::std::unique_ptr< ::Structure >
parseStructure (const ::std::string& u,
                ::xml_schema::ErrorHandler& h,
                ::xml_schema::Flags f,
                const ::xml_schema::Properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::Flags::dont_initialize) == 0,
    (f & ::xml_schema::Flags::keep_dom) == 0);

  ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      u, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  return ::std::unique_ptr< ::Structure > (
    ::parseStructure (
      std::move (d), f | ::xml_schema::Flags::own_dom, p));
}

::std::unique_ptr< ::Structure >
parseStructure (const ::std::string& u,
                ::xercesc::DOMErrorHandler& h,
                ::xml_schema::Flags f,
                const ::xml_schema::Properties& p)
{
  ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      u, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  return ::std::unique_ptr< ::Structure > (
    ::parseStructure (
      std::move (d), f | ::xml_schema::Flags::own_dom, p));
}

::std::unique_ptr< ::Structure >
parseStructure (::std::istream& is,
                ::xml_schema::Flags f,
                const ::xml_schema::Properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::Flags::dont_initialize) == 0,
    (f & ::xml_schema::Flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::parseStructure (isrc, f, p);
}

::std::unique_ptr< ::Structure >
parseStructure (::std::istream& is,
                ::xml_schema::ErrorHandler& h,
                ::xml_schema::Flags f,
                const ::xml_schema::Properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::Flags::dont_initialize) == 0,
    (f & ::xml_schema::Flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::parseStructure (isrc, h, f, p);
}

::std::unique_ptr< ::Structure >
parseStructure (::std::istream& is,
                ::xercesc::DOMErrorHandler& h,
                ::xml_schema::Flags f,
                const ::xml_schema::Properties& p)
{
  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::parseStructure (isrc, h, f, p);
}

::std::unique_ptr< ::Structure >
parseStructure (::std::istream& is,
                const ::std::string& sid,
                ::xml_schema::Flags f,
                const ::xml_schema::Properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::Flags::dont_initialize) == 0,
    (f & ::xml_schema::Flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::parseStructure (isrc, f, p);
}

::std::unique_ptr< ::Structure >
parseStructure (::std::istream& is,
                const ::std::string& sid,
                ::xml_schema::ErrorHandler& h,
                ::xml_schema::Flags f,
                const ::xml_schema::Properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::Flags::dont_initialize) == 0,
    (f & ::xml_schema::Flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::parseStructure (isrc, h, f, p);
}

::std::unique_ptr< ::Structure >
parseStructure (::std::istream& is,
                const ::std::string& sid,
                ::xercesc::DOMErrorHandler& h,
                ::xml_schema::Flags f,
                const ::xml_schema::Properties& p)
{
  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::parseStructure (isrc, h, f, p);
}

::std::unique_ptr< ::Structure >
parseStructure (::xercesc::InputSource& i,
                ::xml_schema::Flags f,
                const ::xml_schema::Properties& p)
{
  ::xsd::cxx::tree::error_handler< char > h;

  ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  h.throw_if_failed< ::xsd::cxx::tree::parsing< char > > ();

  return ::std::unique_ptr< ::Structure > (
    ::parseStructure (
      std::move (d), f | ::xml_schema::Flags::own_dom, p));
}

::std::unique_ptr< ::Structure >
parseStructure (::xercesc::InputSource& i,
                ::xml_schema::ErrorHandler& h,
                ::xml_schema::Flags f,
                const ::xml_schema::Properties& p)
{
  ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  return ::std::unique_ptr< ::Structure > (
    ::parseStructure (
      std::move (d), f | ::xml_schema::Flags::own_dom, p));
}

::std::unique_ptr< ::Structure >
parseStructure (::xercesc::InputSource& i,
                ::xercesc::DOMErrorHandler& h,
                ::xml_schema::Flags f,
                const ::xml_schema::Properties& p)
{
  ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  return ::std::unique_ptr< ::Structure > (
    ::parseStructure (
      std::move (d), f | ::xml_schema::Flags::own_dom, p));
}

::std::unique_ptr< ::Structure >
parseStructure (const ::xercesc::DOMDocument& doc,
                ::xml_schema::Flags f,
                const ::xml_schema::Properties& p)
{
  if (f & ::xml_schema::Flags::keep_dom)
  {
    ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
      static_cast< ::xercesc::DOMDocument* > (doc.cloneNode (true)));

    return ::std::unique_ptr< ::Structure > (
      ::parseStructure (
        std::move (d), f | ::xml_schema::Flags::own_dom, p));
  }

  const ::xercesc::DOMElement& e (*doc.getDocumentElement ());
  const ::xsd::cxx::xml::qualified_name< char > n (
    ::xsd::cxx::xml::dom::name< char > (e));

  if (n.name () == "structure" &&
      n.namespace_ () == "")
  {
    ::std::unique_ptr< ::Structure > r (
      ::xsd::cxx::tree::traits< ::Structure, char >::create (
        e, f, 0));
    return r;
  }

  throw ::xsd::cxx::tree::unexpected_element < char > (
    n.name (),
    n.namespace_ (),
    "structure",
    "");
}

::std::unique_ptr< ::Structure >
parseStructure (::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d,
                ::xml_schema::Flags f,
                const ::xml_schema::Properties&)
{
  ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > c (
    ((f & ::xml_schema::Flags::keep_dom) &&
     !(f & ::xml_schema::Flags::own_dom))
    ? static_cast< ::xercesc::DOMDocument* > (d->cloneNode (true))
    : 0);

  ::xercesc::DOMDocument& doc (c.get () ? *c : *d);
  const ::xercesc::DOMElement& e (*doc.getDocumentElement ());

  const ::xsd::cxx::xml::qualified_name< char > n (
    ::xsd::cxx::xml::dom::name< char > (e));

  if (f & ::xml_schema::Flags::keep_dom)
    doc.setUserData (::xml_schema::dom::treeNodeKey,
                     (c.get () ? &c : &d),
                     0);

  if (n.name () == "structure" &&
      n.namespace_ () == "")
  {
    ::std::unique_ptr< ::Structure > r (
      ::xsd::cxx::tree::traits< ::Structure, char >::create (
        e, f, 0));
    return r;
  }

  throw ::xsd::cxx::tree::unexpected_element < char > (
    n.name (),
    n.namespace_ (),
    "structure",
    "");
}

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

