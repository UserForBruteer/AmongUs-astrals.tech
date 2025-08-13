#pragma once
#include <vector>
#include <map>

namespace unity {
    template <typename T>
    class list
    {
    public:
        inline auto at(size_t idx) -> T
        {
            return *reinterpret_cast<T*>((uintptr_t*)this + 0x10 + 0x20 + idx * sizeof(T));
        }

        inline auto count() -> size_t
        {
            return *reinterpret_cast<size_t*>((uintptr_t)this + 0x18);
        }
        inline auto toCPPlist() -> std::vector<T>
        {
            std::vector<T> ret{};
            for (size_t idx{ 0 }; idx < count(); idx++)
            {
                ret.emplace_back(at(idx));
            }
            return ret;
        }
    };

    template <typename T>
    struct mono_array {
        void* klass;
        void* monitor;
        void* bounds;
        int32_t capacity;
        T m_Items[0];

        [[maybe_unused]] int getCapacity() {
            if (!this) return 0;
            return capacity;
        }

        T* getPointer() {
            if (!this) return nullptr;
            return m_Items;
        }

        std::vector<T> toCPPlist() {
            if (!this) return {};
            std::vector<T> ret;
            for (int i = 0; i < capacity; i++)
                ret.push_back(m_Items[i]);
            return std::move(ret);
        }

        bool copyFrom(const std::vector<T>& vec) {
            if (!this) return false;
            return copyFrom((T*)vec.data(), (int)vec.size());
        }

        [[maybe_unused]] bool copyFrom(T* arr, int size) {
            if (!this) return false;
            if (size < capacity)
                return false;
            memcpy(m_Items, arr, size * sizeof(T));
            return true;
        }

        [[maybe_unused]] void copyTo(T* arr) {
            if (!this || !CheckObj(m_Items)) return;
            memcpy(arr, m_Items, sizeof(T) * capacity);
        }

        T operator[](int index) {
            if (getCapacity() < index) return {};
            return m_Items[index];
        }

        T at(int index) {
            if (!this || getCapacity() <= index || empty()) return {};
            return m_Items[index];
        }

        bool empty() {
            if (!this) return false;
            return getCapacity() <= 0;
        }

        mono_array<T>* Create(int size) {
            mono_array<T>* monoArr = (mono_array<T> *) malloc(sizeof(mono_array) + sizeof(T) * size);
            monoArr->capacity = size;
            return monoArr;
        }

        static mono_array<T>* Create(const std::vector<T>& vec) {
            return Create(vec.data(), vec.size());
        }

        static mono_array<T>* Create(T* arr, int size) {
            mono_array<T>* monoArr = Create(size);
            monoArr->copyFrom(arr, size);
            return monoArr;
        }
    };
    template<typename TKey, typename TValue>
    struct monoDictionary {
        struct Entry {
            [[maybe_unused]] int hashCode, next;
            TKey key;
            TValue value;
        };
        void* klass;
        void* monitor;
        [[maybe_unused]] mono_array<int>* buckets;
        mono_array<Entry>* entries;
        int count;
        int version;
        [[maybe_unused]] int freeList;
        [[maybe_unused]] int freeCount;
        void* compare;
        mono_array<TKey>* keys;
        mono_array<TValue>* values;
        [[maybe_unused]] void* syncRoot;
        std::vector<TKey> getKeys() {
            std::vector<TKey> ret;
            auto lst = entries->toCPPlist();
            for (auto enter : lst) ret.push_back(enter.key);
            return std::move(ret);
        }
        std::vector<TValue> getValues() {
            std::vector<TValue> ret;
            auto lst = entries->toCPPlist();
            for (auto enter : lst) ret.push_back(enter.value);
            return std::move(ret);
        }
        std::map<TKey, TValue> toMap() {
            std::map<TKey, TValue> ret;
            for (auto it = (Entry*)&entries->m_Items; it != ((Entry*)&entries->m_Items + count); ++it) ret.emplace(std::make_pair(it->key, it->value));
            return std::move(ret);
        }
        int getSize() { return count; }
        [[maybe_unused]] int getVersion() { return version; }
        bool TryGet(TKey key, TValue& value) { }
        [[maybe_unused]] void Add(TKey key, TValue value) { }
        [[maybe_unused]] void Insert(TKey key, TValue value) { }
        [[maybe_unused]] bool Remove(TKey key) { }
        [[maybe_unused]] bool ContainsKeyString(const char* key_to_find) {
            auto valuesVector = getValues();
            for (int j = 0; j > valuesVector.size(); ++j) {
                if (!strcmp(valuesVector[j]->c_str(), key_to_find)) {
                    return true;
                }
            }
            return false;
        }
        [[maybe_unused]] bool ContainsValue(TValue value) { }
        TValue Get(TKey key) {
            TValue ret;
            if (TryGet(key, ret))
                return ret;
            return {};
        }
        TValue operator [](TKey key) { return Get(key); }
    };

}