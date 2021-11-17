#include "StringTest.hxx"

#include <string.h>

namespace MyNamespace {
    StringTest::StringTest() {

    }

    StringTest::~StringTest() {

    }

    void StringTest::serialise(char* data, uint64_t& offset) const {
        // Serialise a8String
        // Serialise size of a8String
        uint64_t a8StringSize = a8String.size();
        memcpy(data + offset, &a8StringSize, sizeof(a8StringSize));
        offset += sizeof(a8StringSize);

        // Serialise a8String data
        memcpy(data + offset, &a8String[0], sizeof(a8String[0]) * a8String.size());
        offset += sizeof(a8String[0]) * a8String.size();

        // Serialise b8String
        // Serialise size of b8String
        uint64_t b8StringSize = b8String.size();
        memcpy(data + offset, &b8StringSize, sizeof(b8StringSize));
        offset += sizeof(b8StringSize);

        // Serialise b8String data
        memcpy(data + offset, &b8String[0], sizeof(b8String[0]) * b8String.size());
        offset += sizeof(b8String[0]) * b8String.size();

        // Serialise a16String
        // Serialise size of a16String
        uint64_t a16StringSize = a16String.size();
        memcpy(data + offset, &a16StringSize, sizeof(a16StringSize));
        offset += sizeof(a16StringSize);

        // Serialise a16String data
        memcpy(data + offset, &a16String[0], sizeof(a16String[0]) * a16String.size());
        offset += sizeof(a16String[0]) * a16String.size();

        // Serialise a32String
        // Serialise size of a32String
        uint64_t a32StringSize = a32String.size();
        memcpy(data + offset, &a32StringSize, sizeof(a32StringSize));
        offset += sizeof(a32StringSize);

        // Serialise a32String data
        memcpy(data + offset, &a32String[0], sizeof(a32String[0]) * a32String.size());
        offset += sizeof(a32String[0]) * a32String.size();

        // Serialise a8StringOpt
        // Serialise a8StringOpt presence flag
        *(data + offset++) = a8StringOpt.has_value() ? 1 : 0;

        // Serialise a8StringOpt data
        if (a8StringOpt.has_value()) {
            // Serialise size of a8StringOpt.value()
            uint64_t a8StringOptvalueSize = a8StringOpt.value().size();
            memcpy(data + offset, &a8StringOptvalueSize, sizeof(a8StringOptvalueSize));
            offset += sizeof(a8StringOptvalueSize);

            // Serialise a8StringOpt.value() data
            memcpy(data + offset, &a8StringOpt.value()[0], sizeof(a8StringOpt.value()[0]) * a8StringOpt.value().size());
            offset += sizeof(a8StringOpt.value()[0]) * a8StringOpt.value().size();
        }

        // Serialise b8StringOpt
        // Serialise b8StringOpt presence flag
        *(data + offset++) = b8StringOpt.has_value() ? 1 : 0;

        // Serialise b8StringOpt data
        if (b8StringOpt.has_value()) {
            // Serialise size of b8StringOpt.value()
            uint64_t b8StringOptvalueSize = b8StringOpt.value().size();
            memcpy(data + offset, &b8StringOptvalueSize, sizeof(b8StringOptvalueSize));
            offset += sizeof(b8StringOptvalueSize);

            // Serialise b8StringOpt.value() data
            memcpy(data + offset, &b8StringOpt.value()[0], sizeof(b8StringOpt.value()[0]) * b8StringOpt.value().size());
            offset += sizeof(b8StringOpt.value()[0]) * b8StringOpt.value().size();
        }

        // Serialise a16StringOpt
        // Serialise a16StringOpt presence flag
        *(data + offset++) = a16StringOpt.has_value() ? 1 : 0;

        // Serialise a16StringOpt data
        if (a16StringOpt.has_value()) {
            // Serialise size of a16StringOpt.value()
            uint64_t a16StringOptvalueSize = a16StringOpt.value().size();
            memcpy(data + offset, &a16StringOptvalueSize, sizeof(a16StringOptvalueSize));
            offset += sizeof(a16StringOptvalueSize);

            // Serialise a16StringOpt.value() data
            memcpy(data + offset, &a16StringOpt.value()[0], sizeof(a16StringOpt.value()[0]) * a16StringOpt.value().size());
            offset += sizeof(a16StringOpt.value()[0]) * a16StringOpt.value().size();
        }

        // Serialise a32StringOpt
        // Serialise a32StringOpt presence flag
        *(data + offset++) = a32StringOpt.has_value() ? 1 : 0;

        // Serialise a32StringOpt data
        if (a32StringOpt.has_value()) {
            // Serialise size of a32StringOpt.value()
            uint64_t a32StringOptvalueSize = a32StringOpt.value().size();
            memcpy(data + offset, &a32StringOptvalueSize, sizeof(a32StringOptvalueSize));
            offset += sizeof(a32StringOptvalueSize);

            // Serialise a32StringOpt.value() data
            memcpy(data + offset, &a32StringOpt.value()[0], sizeof(a32StringOpt.value()[0]) * a32StringOpt.value().size());
            offset += sizeof(a32StringOpt.value()[0]) * a32StringOpt.value().size();
        }

        // Serialise array
        for (const auto& e1 : array) {
            // Serialise size of e1
            uint64_t e1Size = e1.size();
            memcpy(data + offset, &e1Size, sizeof(e1Size));
            offset += sizeof(e1Size);

            // Serialise e1 data
            memcpy(data + offset, &e1[0], sizeof(e1[0]) * e1.size());
            offset += sizeof(e1[0]) * e1.size();
        }

        // Serialise arrayOpt
        // Serialise arrayOpt presence flag
        *(data + offset++) = arrayOpt.has_value() ? 1 : 0;

        // Serialise arrayOpt data
        if (arrayOpt.has_value()) {
            for (const auto& e2 : arrayOpt.value()) {
                // Serialise size of e2
                uint64_t e2Size = e2.size();
                memcpy(data + offset, &e2Size, sizeof(e2Size));
                offset += sizeof(e2Size);

                // Serialise e2 data
                memcpy(data + offset, &e2[0], sizeof(e2[0]) * e2.size());
                offset += sizeof(e2[0]) * e2.size();
            }
        }

        // Serialise sequence
        // Serialise size of sequence
        uint64_t sequenceSize = sequence.size();
        memcpy(data + offset, &sequenceSize, sizeof(sequenceSize));
        offset += sizeof(sequenceSize);

        // Serialise sequence data
        for (const auto& e1 : sequence) {
            // Serialise size of e1
            uint64_t e1Size = e1.size();
            memcpy(data + offset, &e1Size, sizeof(e1Size));
            offset += sizeof(e1Size);

            // Serialise e1 data
            memcpy(data + offset, &e1[0], sizeof(e1[0]) * e1.size());
            offset += sizeof(e1[0]) * e1.size();
        }

        // Serialise sequenceOpt
        // Serialise sequenceOpt presence flag
        *(data + offset++) = sequenceOpt.has_value() ? 1 : 0;

        // Serialise sequenceOpt data
        if (sequenceOpt.has_value()) {
            // Serialise size of sequenceOpt.value()
            uint64_t sequenceOptvalueSize = sequenceOpt.value().size();
            memcpy(data + offset, &sequenceOptvalueSize, sizeof(sequenceOptvalueSize));
            offset += sizeof(sequenceOptvalueSize);

            // Serialise sequenceOpt.value() data
            for (const auto& e2 : sequenceOpt.value()) {
                // Serialise size of e2
                uint64_t e2Size = e2.size();
                memcpy(data + offset, &e2Size, sizeof(e2Size));
                offset += sizeof(e2Size);

                // Serialise e2 data
                memcpy(data + offset, &e2[0], sizeof(e2[0]) * e2.size());
                offset += sizeof(e2[0]) * e2.size();
            }
        }

        // Serialise map
        // Serialise size of map
        uint64_t mapSize = map.size();
        memcpy(data + offset, &mapSize, sizeof(mapSize));
        offset += sizeof(mapSize);

        for (const auto& e0 : map) {
            // Serialise map key data
            // Serialise size of e0.first
            uint64_t e0firstSize = e0.first.size();
            memcpy(data + offset, &e0firstSize, sizeof(e0firstSize));
            offset += sizeof(e0firstSize);

            // Serialise e0.first data
            memcpy(data + offset, &e0.first[0], sizeof(e0.first[0]) * e0.first.size());
            offset += sizeof(e0.first[0]) * e0.first.size();

            // Serialise map value data
            // Serialise size of e0.second
            uint64_t e0secondSize = e0.second.size();
            memcpy(data + offset, &e0secondSize, sizeof(e0secondSize));
            offset += sizeof(e0secondSize);

            // Serialise e0.second data
            memcpy(data + offset, &e0.second[0], sizeof(e0.second[0]) * e0.second.size());
            offset += sizeof(e0.second[0]) * e0.second.size();
        }

        // Serialise mapOpt
        // Serialise mapOpt presence flag
        *(data + offset++) = mapOpt.has_value() ? 1 : 0;

        // Serialise mapOpt data
        if (mapOpt.has_value()) {
            // Serialise size of mapOpt.value()
            uint64_t mapOptvalueSize = mapOpt.value().size();
            memcpy(data + offset, &mapOptvalueSize, sizeof(mapOptvalueSize));
            offset += sizeof(mapOptvalueSize);

            for (const auto& e1 : mapOpt.value()) {
                // Serialise mapOptvalue key data
                // Serialise size of e1.first
                uint64_t e1firstSize = e1.first.size();
                memcpy(data + offset, &e1firstSize, sizeof(e1firstSize));
                offset += sizeof(e1firstSize);

                // Serialise e1.first data
                memcpy(data + offset, &e1.first[0], sizeof(e1.first[0]) * e1.first.size());
                offset += sizeof(e1.first[0]) * e1.first.size();

                // Serialise mapOptvalue value data
                // Serialise size of e1.second
                uint64_t e1secondSize = e1.second.size();
                memcpy(data + offset, &e1secondSize, sizeof(e1secondSize));
                offset += sizeof(e1secondSize);

                // Serialise e1.second data
                memcpy(data + offset, &e1.second[0], sizeof(e1.second[0]) * e1.second.size());
                offset += sizeof(e1.second[0]) * e1.second.size();
            }
        }
    }

    void StringTest::deserialise(const char* data, uint64_t& offset) {
        // Deserialise a8String
        // Deserialise size of a8String
        uint64_t a8StringSize;
        memcpy(&a8StringSize, data + offset, sizeof(a8StringSize));
        offset += sizeof(a8StringSize);
        a8String.resize(a8StringSize);

        // Deserialise a8String data
        memcpy(&a8String[0], data + offset, sizeof(a8String[0]) * a8String.size());
        offset += sizeof(a8String[0]) * a8String.size();

        // Deserialise b8String
        // Deserialise size of b8String
        uint64_t b8StringSize;
        memcpy(&b8StringSize, data + offset, sizeof(b8StringSize));
        offset += sizeof(b8StringSize);
        b8String.resize(b8StringSize);

        // Deserialise b8String data
        memcpy(&b8String[0], data + offset, sizeof(b8String[0]) * b8String.size());
        offset += sizeof(b8String[0]) * b8String.size();

        // Deserialise a16String
        // Deserialise size of a16String
        uint64_t a16StringSize;
        memcpy(&a16StringSize, data + offset, sizeof(a16StringSize));
        offset += sizeof(a16StringSize);
        a16String.resize(a16StringSize);

        // Deserialise a16String data
        memcpy(&a16String[0], data + offset, sizeof(a16String[0]) * a16String.size());
        offset += sizeof(a16String[0]) * a16String.size();

        // Deserialise a32String
        // Deserialise size of a32String
        uint64_t a32StringSize;
        memcpy(&a32StringSize, data + offset, sizeof(a32StringSize));
        offset += sizeof(a32StringSize);
        a32String.resize(a32StringSize);

        // Deserialise a32String data
        memcpy(&a32String[0], data + offset, sizeof(a32String[0]) * a32String.size());
        offset += sizeof(a32String[0]) * a32String.size();

        // Deserialise a8StringOpt
        if (*(data + offset++) == 1) {
            a8StringOpt = typename std::decay_t<decltype(a8StringOpt)>::value_type{};
            // Deserialise size of a8StringOpt.value()
            uint64_t a8StringOptvalueSize;
            memcpy(&a8StringOptvalueSize, data + offset, sizeof(a8StringOptvalueSize));
            offset += sizeof(a8StringOptvalueSize);
            a8StringOpt.value().resize(a8StringOptvalueSize);

            // Deserialise a8StringOpt.value() data
            memcpy(&a8StringOpt.value()[0], data + offset, sizeof(a8StringOpt.value()[0]) * a8StringOpt.value().size());
            offset += sizeof(a8StringOpt.value()[0]) * a8StringOpt.value().size();
        } else {
            a8StringOpt.reset();
        }

        // Deserialise b8StringOpt
        if (*(data + offset++) == 1) {
            b8StringOpt = typename std::decay_t<decltype(b8StringOpt)>::value_type{};
            // Deserialise size of b8StringOpt.value()
            uint64_t b8StringOptvalueSize;
            memcpy(&b8StringOptvalueSize, data + offset, sizeof(b8StringOptvalueSize));
            offset += sizeof(b8StringOptvalueSize);
            b8StringOpt.value().resize(b8StringOptvalueSize);

            // Deserialise b8StringOpt.value() data
            memcpy(&b8StringOpt.value()[0], data + offset, sizeof(b8StringOpt.value()[0]) * b8StringOpt.value().size());
            offset += sizeof(b8StringOpt.value()[0]) * b8StringOpt.value().size();
        } else {
            b8StringOpt.reset();
        }

        // Deserialise a16StringOpt
        if (*(data + offset++) == 1) {
            a16StringOpt = typename std::decay_t<decltype(a16StringOpt)>::value_type{};
            // Deserialise size of a16StringOpt.value()
            uint64_t a16StringOptvalueSize;
            memcpy(&a16StringOptvalueSize, data + offset, sizeof(a16StringOptvalueSize));
            offset += sizeof(a16StringOptvalueSize);
            a16StringOpt.value().resize(a16StringOptvalueSize);

            // Deserialise a16StringOpt.value() data
            memcpy(&a16StringOpt.value()[0], data + offset, sizeof(a16StringOpt.value()[0]) * a16StringOpt.value().size());
            offset += sizeof(a16StringOpt.value()[0]) * a16StringOpt.value().size();
        } else {
            a16StringOpt.reset();
        }

        // Deserialise a32StringOpt
        if (*(data + offset++) == 1) {
            a32StringOpt = typename std::decay_t<decltype(a32StringOpt)>::value_type{};
            // Deserialise size of a32StringOpt.value()
            uint64_t a32StringOptvalueSize;
            memcpy(&a32StringOptvalueSize, data + offset, sizeof(a32StringOptvalueSize));
            offset += sizeof(a32StringOptvalueSize);
            a32StringOpt.value().resize(a32StringOptvalueSize);

            // Deserialise a32StringOpt.value() data
            memcpy(&a32StringOpt.value()[0], data + offset, sizeof(a32StringOpt.value()[0]) * a32StringOpt.value().size());
            offset += sizeof(a32StringOpt.value()[0]) * a32StringOpt.value().size();
        } else {
            a32StringOpt.reset();
        }

        // Deserialise array
        for (auto& e1 : array) {
            // Deserialise size of e1
            uint64_t e1Size;
            memcpy(&e1Size, data + offset, sizeof(e1Size));
            offset += sizeof(e1Size);
            e1.resize(e1Size);

            // Deserialise e1 data
            memcpy(&e1[0], data + offset, sizeof(e1[0]) * e1.size());
            offset += sizeof(e1[0]) * e1.size();
        }

        // Deserialise arrayOpt
        if (*(data + offset++) == 1) {
            arrayOpt = typename std::decay_t<decltype(arrayOpt)>::value_type{};
            for (auto& e2 : arrayOpt.value()) {
                // Deserialise size of e2
                uint64_t e2Size;
                memcpy(&e2Size, data + offset, sizeof(e2Size));
                offset += sizeof(e2Size);
                e2.resize(e2Size);

                // Deserialise e2 data
                memcpy(&e2[0], data + offset, sizeof(e2[0]) * e2.size());
                offset += sizeof(e2[0]) * e2.size();
            }
        } else {
            arrayOpt.reset();
        }

        // Deserialise sequence
        // Deserialise size of sequence
        uint64_t sequenceSize;
        memcpy(&sequenceSize, data + offset, sizeof(sequenceSize));
        offset += sizeof(sequenceSize);

        // Deserialise sequence data
        sequence.resize(sequenceSize);
        for (auto& e1 : sequence) {
            // Deserialise size of e1
            uint64_t e1Size;
            memcpy(&e1Size, data + offset, sizeof(e1Size));
            offset += sizeof(e1Size);
            e1.resize(e1Size);

            // Deserialise e1 data
            memcpy(&e1[0], data + offset, sizeof(e1[0]) * e1.size());
            offset += sizeof(e1[0]) * e1.size();
        }

        // Deserialise sequenceOpt
        if (*(data + offset++) == 1) {
            sequenceOpt = typename std::decay_t<decltype(sequenceOpt)>::value_type{};
            // Deserialise size of sequenceOpt.value()
            uint64_t sequenceOptvalueSize;
            memcpy(&sequenceOptvalueSize, data + offset, sizeof(sequenceOptvalueSize));
            offset += sizeof(sequenceOptvalueSize);

            // Deserialise sequenceOpt.value() data
            sequenceOpt.value().resize(sequenceOptvalueSize);
            for (auto& e2 : sequenceOpt.value()) {
                // Deserialise size of e2
                uint64_t e2Size;
                memcpy(&e2Size, data + offset, sizeof(e2Size));
                offset += sizeof(e2Size);
                e2.resize(e2Size);

                // Deserialise e2 data
                memcpy(&e2[0], data + offset, sizeof(e2[0]) * e2.size());
                offset += sizeof(e2[0]) * e2.size();
            }
        } else {
            sequenceOpt.reset();
        }

        // Deserialise map
        // Deserialise size of map
        uint64_t mapSize;
        memcpy(&mapSize, data + offset, sizeof(mapSize));
        offset += sizeof(mapSize);

        // Deserialise map data
        for (uint64_t index = 0; index < mapSize; ++index) {
            // Deserialise map key data
            std::string first0;
            // Deserialise size of first0
            uint64_t first0Size;
            memcpy(&first0Size, data + offset, sizeof(first0Size));
            offset += sizeof(first0Size);
            first0.resize(first0Size);

            // Deserialise first0 data
            memcpy(&first0[0], data + offset, sizeof(first0[0]) * first0.size());
            offset += sizeof(first0[0]) * first0.size();

            // Deserialise map value data
            std::u32string second0;
            // Deserialise size of second0
            uint64_t second0Size;
            memcpy(&second0Size, data + offset, sizeof(second0Size));
            offset += sizeof(second0Size);
            second0.resize(second0Size);

            // Deserialise second0 data
            memcpy(&second0[0], data + offset, sizeof(second0[0]) * second0.size());
            offset += sizeof(second0[0]) * second0.size();

            map.insert({first0, second0});
        }

        // Deserialise mapOpt
        if (*(data + offset++) == 1) {
            mapOpt = typename std::decay_t<decltype(mapOpt)>::value_type{};
            // Deserialise size of mapOpt.value()
            uint64_t mapOptvalueSize;
            memcpy(&mapOptvalueSize, data + offset, sizeof(mapOptvalueSize));
            offset += sizeof(mapOptvalueSize);

            // Deserialise mapOpt.value() data
            for (uint64_t index = 0; index < mapOptvalueSize; ++index) {
                // Deserialise mapOptvalue key data
                std::string first1;
                // Deserialise size of first1
                uint64_t first1Size;
                memcpy(&first1Size, data + offset, sizeof(first1Size));
                offset += sizeof(first1Size);
                first1.resize(first1Size);

                // Deserialise first1 data
                memcpy(&first1[0], data + offset, sizeof(first1[0]) * first1.size());
                offset += sizeof(first1[0]) * first1.size();

                // Deserialise mapOptvalue value data
                std::u32string second1;
                // Deserialise size of second1
                uint64_t second1Size;
                memcpy(&second1Size, data + offset, sizeof(second1Size));
                offset += sizeof(second1Size);
                second1.resize(second1Size);

                // Deserialise second1 data
                memcpy(&second1[0], data + offset, sizeof(second1[0]) * second1.size());
                offset += sizeof(second1[0]) * second1.size();

                mapOpt.value().insert({first1, second1});
            }
        } else {
            mapOpt.reset();
        }
    }

    uint64_t StringTest::getSizeInBytes() const {
        uint64_t size = 0;

        // Add on size of a8String
        size += sizeof(std::string::value_type) * a8String.size();

        // Add on size of string size field
        size += sizeof(uint64_t);

        // Add on size of b8String
        size += sizeof(std::string::value_type) * b8String.size();

        // Add on size of string size field
        size += sizeof(uint64_t);

        // Add on size of a16String
        size += sizeof(std::u16string::value_type) * a16String.size();

        // Add on size of string size field
        size += sizeof(uint64_t);

        // Add on size of a32String
        size += sizeof(std::u32string::value_type) * a32String.size();

        // Add on size of string size field
        size += sizeof(uint64_t);

        // Add on size of a8StringOpt
        // Optionality flag for a8StringOpt = 1 byte
        ++size;

        // If a8StringOpt is present then add on size of it
        if (a8StringOpt.has_value()) {
            size += sizeof(std::string::value_type) * a8StringOpt.value().size();

            // Add on size of string size field
            size += sizeof(uint64_t);
        }

        // Add on size of b8StringOpt
        // Optionality flag for b8StringOpt = 1 byte
        ++size;

        // If b8StringOpt is present then add on size of it
        if (b8StringOpt.has_value()) {
            size += sizeof(std::string::value_type) * b8StringOpt.value().size();

            // Add on size of string size field
            size += sizeof(uint64_t);
        }

        // Add on size of a16StringOpt
        // Optionality flag for a16StringOpt = 1 byte
        ++size;

        // If a16StringOpt is present then add on size of it
        if (a16StringOpt.has_value()) {
            size += sizeof(std::u16string::value_type) * a16StringOpt.value().size();

            // Add on size of string size field
            size += sizeof(uint64_t);
        }

        // Add on size of a32StringOpt
        // Optionality flag for a32StringOpt = 1 byte
        ++size;

        // If a32StringOpt is present then add on size of it
        if (a32StringOpt.has_value()) {
            size += sizeof(std::u32string::value_type) * a32StringOpt.value().size();

            // Add on size of string size field
            size += sizeof(uint64_t);
        }

        // Add on size of array
        for (const auto& e1 : array) {
            // Add on size of each individual std::u16string
            size += sizeof(std::u16string::value_type) * e1.size();

            // Add on size of string size field
            size += sizeof(uint64_t);
        }

        // Add on size of arrayOpt
        // Optionality flag for arrayOpt = 1 byte
        ++size;

        // If arrayOpt is present then add on size of it
        if (arrayOpt.has_value()) {
            for (const auto& e2 : arrayOpt.value()) {
                // Add on size of each individual std::u16string
                size += sizeof(std::u16string::value_type) * e2.size();

                // Add on size of string size field
                size += sizeof(uint64_t);
            }
        }

        // Add on size of sequence
        // Add on size of sequence length field
        size += sizeof(uint64_t);

        // Add on size of sequence data
        for (const auto& e1 : sequence) {
            // Add on size of each individual std::u32string
            size += sizeof(std::u32string::value_type) * e1.size();

            // Add on size of string size field
            size += sizeof(uint64_t);
        }

        // Add on size of sequenceOpt
        // Optionality flag for sequenceOpt = 1 byte
        ++size;

        // If sequenceOpt is present then add on size of it
        if (sequenceOpt.has_value()) {
            // Add on size of sequenceOpt.value() length field
            size += sizeof(uint64_t);

            // Add on size of sequenceOpt.value() data
            for (const auto& e2 : sequenceOpt.value()) {
                // Add on size of each individual std::u32string
                size += sizeof(std::u32string::value_type) * e2.size();

                // Add on size of string size field
                size += sizeof(uint64_t);
            }
        }

        // Add on size of map
        // Add on size of map length field
        size += sizeof(uint64_t);

        // Add on size of map key data
        for (const auto& e1 : map) {
            // Add on size of each individual std::string
            size += sizeof(std::string::value_type) * e1.first.size();

            // Add on size of string size field
            size += sizeof(uint64_t);
        }

        // Add on size of map value data
        for (const auto& e1 : map) {
            // Add on size of each individual std::u32string
            size += sizeof(std::u32string::value_type) * e1.second.size();

            // Add on size of string size field
            size += sizeof(uint64_t);
        }

        // Add on size of mapOpt
        // Optionality flag for mapOpt = 1 byte
        ++size;

        // If mapOpt is present then add on size of it
        if (mapOpt.has_value()) {
            // Add on size of mapOpt.value() length field
            size += sizeof(uint64_t);

            // Add on size of mapOpt.value() key data
            for (const auto& e2 : mapOpt.value()) {
                // Add on size of each individual std::string
                size += sizeof(std::string::value_type) * e2.first.size();

                // Add on size of string size field
                size += sizeof(uint64_t);
            }

            // Add on size of mapOpt.value() value data
            for (const auto& e2 : mapOpt.value()) {
                // Add on size of each individual std::u32string
                size += sizeof(std::u32string::value_type) * e2.second.size();

                // Add on size of string size field
                size += sizeof(uint64_t);
            }
        }

        return size;
    }

    std::string& StringTest::getA8String() {
        return a8String;
    }

    std::string& StringTest::getB8String() {
        return b8String;
    }

    std::u16string& StringTest::getA16String() {
        return a16String;
    }

    std::u32string& StringTest::getA32String() {
        return a32String;
    }

    std::optional<std::string>& StringTest::getA8StringOpt() {
        return a8StringOpt;
    }

    std::optional<std::string>& StringTest::getB8StringOpt() {
        return b8StringOpt;
    }

    std::optional<std::u16string>& StringTest::getA16StringOpt() {
        return a16StringOpt;
    }

    std::optional<std::u32string>& StringTest::getA32StringOpt() {
        return a32StringOpt;
    }

    std::array<std::u16string, 7>& StringTest::getArray() {
        return array;
    }

    std::optional<std::array<std::u16string, 7>>& StringTest::getArrayOpt() {
        return arrayOpt;
    }

    std::vector<std::u32string>& StringTest::getSequence() {
        return sequence;
    }

    std::optional<std::vector<std::u32string>>& StringTest::getSequenceOpt() {
        return sequenceOpt;
    }

    std::map<std::string, std::u32string>& StringTest::getMap() {
        return map;
    }

    std::optional<std::map<std::string, std::u32string>>& StringTest::getMapOpt() {
        return mapOpt;
    }

    void StringTest::setA8String(const std::string& value) {
        a8String = value;
    }

    void StringTest::setB8String(const std::string& value) {
        b8String = value;
    }

    void StringTest::setA16String(const std::u16string& value) {
        a16String = value;
    }

    void StringTest::setA32String(const std::u32string& value) {
        a32String = value;
    }

    void StringTest::setA8StringOpt(const std::string& value) {
        a8StringOpt = value;
    }

    void StringTest::setB8StringOpt(const std::string& value) {
        b8StringOpt = value;
    }

    void StringTest::setA16StringOpt(const std::u16string& value) {
        a16StringOpt = value;
    }

    void StringTest::setA32StringOpt(const std::u32string& value) {
        a32StringOpt = value;
    }

}
