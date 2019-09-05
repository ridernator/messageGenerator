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
    for (const auto& arrayOfIntSequencesa : arrayOfIntSequences) {
        Serialiser::serialisePrimitiveSequence(data, arrayOfIntSequencesa, offset);
    }

    // Serialise sequenceOfIntArrays
    for (const auto& sequenceOfIntArraysa : sequenceOfIntArrays) {
        Serialiser::serialisePrimitiveArray(data, sequenceOfIntArraysa, offset);
    }

    // Serialise intToDoubleMap
    // Don't know what to do with map=intToDoubleMap

    // Serialise intArray5d
    for (const auto& intArray5da : intArray5d) {
        for (const auto& intArray5daa : intArray5da) {
            for (const auto& intArray5daaa : intArray5daa) {
                for (const auto& intArray5daaaa : intArray5daaa) {
                    Serialiser::serialisePrimitiveArray(data, intArray5daaaa, offset);
                }
            }
        }
    }

    // Serialise floatSequence5d
    for (const auto& floatSequence5da : floatSequence5d) {
        for (const auto& floatSequence5daa : floatSequence5da) {
            for (const auto& floatSequence5daaa : floatSequence5daa) {
                for (const auto& floatSequence5daaaa : floatSequence5daaa) {
                    Serialiser::serialisePrimitiveSequence(data, floatSequence5daaaa, offset);
                }
            }
        }
    }

    // Serialise blobSequence5d
    for (const auto& blobSequence5da : blobSequence5d) {
        for (const auto& blobSequence5daa : blobSequence5da) {
            for (const auto& blobSequence5daaa : blobSequence5daa) {
                for (const auto& blobSequence5daaaa : blobSequence5daaa) {
                    for (const auto& blobSequence5daaaaa : blobSequence5daaaa) {
                        blobSequence5daaaaa.serialise(data, offset);
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
    for (auto& arrayOfIntSequencesa : arrayOfIntSequences) {
        Serialiser::deserialisePrimitiveSequence(data, arrayOfIntSequencesa, offset);
    }

    // Deserialise sequenceOfIntArrays
    for (auto& sequenceOfIntArraysa : sequenceOfIntArrays) {
        Serialiser::deserialisePrimitiveArray(data, sequenceOfIntArraysa, offset);
    }

    // Deserialise intToDoubleMap
    // Don't know what to do with map=intToDoubleMap

    // Deserialise intArray5d
    for (auto& intArray5da : intArray5d) {
        for (auto& intArray5daa : intArray5da) {
            for (auto& intArray5daaa : intArray5daa) {
                for (auto& intArray5daaaa : intArray5daaa) {
                    Serialiser::deserialisePrimitiveArray(data, intArray5daaaa, offset);
                }
            }
        }
    }

    // Deserialise floatSequence5d
    for (auto& floatSequence5da : floatSequence5d) {
        for (auto& floatSequence5daa : floatSequence5da) {
            for (auto& floatSequence5daaa : floatSequence5daa) {
                for (auto& floatSequence5daaaa : floatSequence5daaa) {
                    Serialiser::deserialisePrimitiveSequence(data, floatSequence5daaaa, offset);
                }
            }
        }
    }

    // Deserialise blobSequence5d
    for (auto& blobSequence5da : blobSequence5d) {
        for (auto& blobSequence5daa : blobSequence5da) {
            for (auto& blobSequence5daaa : blobSequence5daa) {
                for (auto& blobSequence5daaaa : blobSequence5daaa) {
                    for (auto& blobSequence5daaaaa : blobSequence5daaaa) {
                        blobSequence5daaaaa.deserialise(data, offset);
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
    for (const auto& arrayOfIntSequencesa : arrayOfIntSequences) {
        size += (arrayOfIntSequencesa.size() * sizeof(int32_t)) + sizeof(arrayOfIntSequencesa.size());
    }

    // Calculate size of sequenceOfIntArrays
    for (const auto& sequenceOfIntArraysa : sequenceOfIntArrays) {
        size += 10 * sizeof(int32_t);
    }
    // Extra field for size of sequenceOfIntArrayssequence
    size += sizeof(sequenceOfIntArrays.size());

    // Calculate size of intToDoubleMap
    // Don't know what to do with map=intToDoubleMap

    // Calculate size of intArray5d
    for (const auto& intArray5da : intArray5d) {
        for (const auto& intArray5daa : intArray5da) {
            for (const auto& intArray5daaa : intArray5daa) {
                for (const auto& intArray5daaaa : intArray5daaa) {
                    size += 6 * sizeof(int32_t);
                }
            }
        }
    }

    // Calculate size of floatSequence5d
    for (const auto& floatSequence5da : floatSequence5d) {
        for (const auto& floatSequence5daa : floatSequence5da) {
            for (const auto& floatSequence5daaa : floatSequence5daa) {
                for (const auto& floatSequence5daaaa : floatSequence5daaa) {
                    size += (floatSequence5daaaa.size() * sizeof(float)) + sizeof(floatSequence5daaaa.size());
                }
                // Extra field for size of floatSequence5daaasequence
                size += sizeof(floatSequence5daaa.size());
            }
            // Extra field for size of floatSequence5daasequence
            size += sizeof(floatSequence5daa.size());
        }
        // Extra field for size of floatSequence5dasequence
        size += sizeof(floatSequence5da.size());
    }
    // Extra field for size of floatSequence5dsequence
    size += sizeof(floatSequence5d.size());

    // Calculate size of blobSequence5d
    for (const auto& blobSequence5da : blobSequence5d) {
        for (const auto& blobSequence5daa : blobSequence5da) {
            for (const auto& blobSequence5daaa : blobSequence5daa) {
                for (const auto& blobSequence5daaaa : blobSequence5daaa) {
                    for (const auto& blobSequence5daaaaa : blobSequence5daaaa) {
                        size += blobSequence5daaaaa.getSizeInBytes();
                    }
                    // Extra field for size of blobSequence5daaaasequence
                    size += sizeof(blobSequence5daaaa.size());
                }
                // Extra field for size of blobSequence5daaasequence
                size += sizeof(blobSequence5daaa.size());
            }
            // Extra field for size of blobSequence5daasequence
            size += sizeof(blobSequence5daa.size());
        }
        // Extra field for size of blobSequence5dasequence
        size += sizeof(blobSequence5da.size());
    }
    // Extra field for size of blobSequence5dsequence
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

