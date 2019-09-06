#include "StressTest.hxx"

StressTest::StressTest() {

}

StressTest::~StressTest() {

}

void StressTest::serialise(char* data, uint64_t& offset) const {
    // Serialise simpleInt
    Serialiser::serialisePrimitive(data, simpleInt, offset);

    // Serialise blobber
    blobber.serialise(data, offset);

    // Serialise stringer
    Serialiser::serialiseString(data, stringer, offset);

    // Serialise simpleIntNoDoc
    Serialiser::serialisePrimitive(data, simpleIntNoDoc, offset);

    // Serialise simpleFloat
    Serialiser::serialisePrimitive(data, simpleFloat, offset);

    // Serialise intArray
    Serialiser::serialisePrimitiveArray(data, intArray, offset);

    // Serialise floatSequence
    Serialiser::serialisePrimitiveSequence(data, floatSequence, offset);

    // Serialise arrayOfIntSequences
    for (const auto& arrayOfIntSequencesMember : arrayOfIntSequences) {
        Serialiser::serialisePrimitiveSequence(data, arrayOfIntSequencesMember, offset);
    }

    // Serialise sequenceOfIntArrays
    for (const auto& sequenceOfIntArraysMember : sequenceOfIntArrays) {
        Serialiser::serialisePrimitiveArray(data, sequenceOfIntArraysMember, offset);
    }

    // Serialise intToDoubleMap
    // Don't know what to do with map=intToDoubleMap

    // Serialise intArray5d
    for (const auto& intArray5dMember : intArray5d) {
        for (const auto& intArray5dMemberMember : intArray5dMember) {
            for (const auto& intArray5dMemberMemberMember : intArray5dMemberMember) {
                for (const auto& intArray5dMemberMemberMemberMember : intArray5dMemberMemberMember) {
                    Serialiser::serialisePrimitiveArray(data, intArray5dMemberMemberMemberMember, offset);
                }
            }
        }
    }

    // Serialise floatSequence5d
    for (const auto& floatSequence5dMember : floatSequence5d) {
        for (const auto& floatSequence5dMemberMember : floatSequence5dMember) {
            for (const auto& floatSequence5dMemberMemberMember : floatSequence5dMemberMember) {
                for (const auto& floatSequence5dMemberMemberMemberMember : floatSequence5dMemberMemberMember) {
                    Serialiser::serialisePrimitiveSequence(data, floatSequence5dMemberMemberMemberMember, offset);
                }
            }
        }
    }

    // Serialise blobSequence5d
    for (const auto& blobSequence5dMember : blobSequence5d) {
        for (const auto& blobSequence5dMemberMember : blobSequence5dMember) {
            for (const auto& blobSequence5dMemberMemberMember : blobSequence5dMemberMember) {
                for (const auto& blobSequence5dMemberMemberMemberMember : blobSequence5dMemberMemberMember) {
                    for (const auto& blobSequence5dMemberMemberMemberMemberMember : blobSequence5dMemberMemberMemberMember) {
                        blobSequence5dMemberMemberMemberMemberMember.serialise(data, offset);
                    }
                }
            }
        }
    }
}

void StressTest::deserialise(const char* data, uint64_t& offset) {
    // Deserialise simpleInt
    Serialiser::deserialisePrimitive(data, simpleInt, offset);

    // Deserialise blobber
    blobber.deserialise(data, offset);

    // Deserialise stringer
    Serialiser::deserialiseString(data, stringer, offset);

    // Deserialise simpleIntNoDoc
    Serialiser::deserialisePrimitive(data, simpleIntNoDoc, offset);

    // Deserialise simpleFloat
    Serialiser::deserialisePrimitive(data, simpleFloat, offset);

    // Deserialise intArray
    Serialiser::deserialisePrimitiveArray(data, intArray, offset);

    // Deserialise floatSequence
    Serialiser::deserialisePrimitiveSequence(data, floatSequence, offset);

    // Deserialise arrayOfIntSequences
    for (auto& arrayOfIntSequencesMember : arrayOfIntSequences) {
        Serialiser::deserialisePrimitiveSequence(data, arrayOfIntSequencesMember, offset);
    }

    // Deserialise sequenceOfIntArrays
    for (auto& sequenceOfIntArraysMember : sequenceOfIntArrays) {
        Serialiser::deserialisePrimitiveArray(data, sequenceOfIntArraysMember, offset);
    }

    // Deserialise intToDoubleMap
    // Don't know what to do with map=intToDoubleMap

    // Deserialise intArray5d
    for (auto& intArray5dMember : intArray5d) {
        for (auto& intArray5dMemberMember : intArray5dMember) {
            for (auto& intArray5dMemberMemberMember : intArray5dMemberMember) {
                for (auto& intArray5dMemberMemberMemberMember : intArray5dMemberMemberMember) {
                    Serialiser::deserialisePrimitiveArray(data, intArray5dMemberMemberMemberMember, offset);
                }
            }
        }
    }

    // Deserialise floatSequence5d
    for (auto& floatSequence5dMember : floatSequence5d) {
        for (auto& floatSequence5dMemberMember : floatSequence5dMember) {
            for (auto& floatSequence5dMemberMemberMember : floatSequence5dMemberMember) {
                for (auto& floatSequence5dMemberMemberMemberMember : floatSequence5dMemberMemberMember) {
                    Serialiser::deserialisePrimitiveSequence(data, floatSequence5dMemberMemberMemberMember, offset);
                }
            }
        }
    }

    // Deserialise blobSequence5d
    for (auto& blobSequence5dMember : blobSequence5d) {
        for (auto& blobSequence5dMemberMember : blobSequence5dMember) {
            for (auto& blobSequence5dMemberMemberMember : blobSequence5dMemberMember) {
                for (auto& blobSequence5dMemberMemberMemberMember : blobSequence5dMemberMemberMember) {
                    for (auto& blobSequence5dMemberMemberMemberMemberMember : blobSequence5dMemberMemberMemberMember) {
                        blobSequence5dMemberMemberMemberMemberMember.deserialise(data, offset);
                    }
                }
            }
        }
    }
}

uint64_t StressTest::getSizeInBytes() const {
    // Size of primitive types in this structure
    uint64_t size = 12;

    // Calculate size of blobber
    size += blobber.getSizeInBytes();

    // Calculate size of stringer
    size += stringer.size() + sizeof(stringer.size());

    // Calculate size of intArray
    size += SIZE_OF_INT_ARRAY * sizeof(int32_t);

    // Calculate size of floatSequence
    size += (floatSequence.size() * sizeof(float)) + sizeof(floatSequence.size());

    // Calculate size of arrayOfIntSequences
    for (const auto& arrayOfIntSequencesMember : arrayOfIntSequences) {
        size += (arrayOfIntSequencesMember.size() * sizeof(int32_t)) + sizeof(arrayOfIntSequencesMember.size());
    }

    // Calculate size of sequenceOfIntArrays
    for (const auto& sequenceOfIntArraysMember : sequenceOfIntArrays) {
        size += 10 * sizeof(int32_t);
    }
    // Extra field for size of sequenceOfIntArrays sequence
    size += sizeof(sequenceOfIntArrays.size());

    // Calculate size of intToDoubleMap
    // Don't know what to do with map=intToDoubleMap

    // Calculate size of intArray5d
    for (const auto& intArray5dMember : intArray5d) {
        for (const auto& intArray5dMemberMember : intArray5dMember) {
            for (const auto& intArray5dMemberMemberMember : intArray5dMemberMember) {
                for (const auto& intArray5dMemberMemberMemberMember : intArray5dMemberMemberMember) {
                    size += 6 * sizeof(int32_t);
                }
            }
        }
    }

    // Calculate size of floatSequence5d
    for (const auto& floatSequence5dMember : floatSequence5d) {
        for (const auto& floatSequence5dMemberMember : floatSequence5dMember) {
            for (const auto& floatSequence5dMemberMemberMember : floatSequence5dMemberMember) {
                for (const auto& floatSequence5dMemberMemberMemberMember : floatSequence5dMemberMemberMember) {
                    size += (floatSequence5dMemberMemberMemberMember.size() * sizeof(float)) + sizeof(floatSequence5dMemberMemberMemberMember.size());
                }
                // Extra field for size of floatSequence5dMemberMemberMember sequence
                size += sizeof(floatSequence5dMemberMemberMember.size());
            }
            // Extra field for size of floatSequence5dMemberMember sequence
            size += sizeof(floatSequence5dMemberMember.size());
        }
        // Extra field for size of floatSequence5dMember sequence
        size += sizeof(floatSequence5dMember.size());
    }
    // Extra field for size of floatSequence5d sequence
    size += sizeof(floatSequence5d.size());

    // Calculate size of blobSequence5d
    for (const auto& blobSequence5dMember : blobSequence5d) {
        for (const auto& blobSequence5dMemberMember : blobSequence5dMember) {
            for (const auto& blobSequence5dMemberMemberMember : blobSequence5dMemberMember) {
                for (const auto& blobSequence5dMemberMemberMemberMember : blobSequence5dMemberMemberMember) {
                    for (const auto& blobSequence5dMemberMemberMemberMemberMember : blobSequence5dMemberMemberMemberMember) {
                        size += blobSequence5dMemberMemberMemberMemberMember.getSizeInBytes();
                    }
                    // Extra field for size of blobSequence5dMemberMemberMemberMember sequence
                    size += sizeof(blobSequence5dMemberMemberMemberMember.size());
                }
                // Extra field for size of blobSequence5dMemberMemberMember sequence
                size += sizeof(blobSequence5dMemberMemberMember.size());
            }
            // Extra field for size of blobSequence5dMemberMember sequence
            size += sizeof(blobSequence5dMemberMember.size());
        }
        // Extra field for size of blobSequence5dMember sequence
        size += sizeof(blobSequence5dMember.size());
    }
    // Extra field for size of blobSequence5d sequence
    size += sizeof(blobSequence5d.size());

    return size;
}

decltype(StressTest::simpleInt) StressTest::getSimpleInt() {
    return simpleInt;
}

decltype(StressTest::blobber)& StressTest::getBlobber() {
    return blobber;
}

decltype(StressTest::stringer)& StressTest::getStringer() {
    return stringer;
}

decltype(StressTest::simpleIntNoDoc) StressTest::getSimpleIntNoDoc() {
    return simpleIntNoDoc;
}

decltype(StressTest::simpleFloat) StressTest::getSimpleFloat() {
    return simpleFloat;
}

decltype(StressTest::intArray)& StressTest::getIntArray() {
    return intArray;
}

decltype(StressTest::floatSequence)& StressTest::getFloatSequence() {
    return floatSequence;
}

decltype(StressTest::arrayOfIntSequences)& StressTest::getArrayOfIntSequences() {
    return arrayOfIntSequences;
}

decltype(StressTest::sequenceOfIntArrays)& StressTest::getSequenceOfIntArrays() {
    return sequenceOfIntArrays;
}

decltype(StressTest::intToDoubleMap)& StressTest::getIntToDoubleMap() {
    return intToDoubleMap;
}

decltype(StressTest::intArray5d)& StressTest::getIntArray5d() {
    return intArray5d;
}

decltype(StressTest::floatSequence5d)& StressTest::getFloatSequence5d() {
    return floatSequence5d;
}

decltype(StressTest::blobSequence5d)& StressTest::getBlobSequence5d() {
    return blobSequence5d;
}

void StressTest::setSimpleInt(const decltype(StressTest::simpleInt) value) {
    simpleInt = value;
}

void StressTest::setSimpleIntNoDoc(const decltype(StressTest::simpleIntNoDoc) value) {
    simpleIntNoDoc = value;
}

void StressTest::setSimpleFloat(const decltype(StressTest::simpleFloat) value) {
    simpleFloat = value;
}

