#pragma once

#include <base/common/base_define.h>

#include <string>
#include <stdexcept>
#include <algorithm>

BASE_BEGIN_NAMESPACE

/**
 * @brief   用于处理只读字符串的轻量对象
 * @ps      C++17 中已有 std::string_view 类型
 */
template<typename CharT, typename Traits>
class basic_string_view
{
public:
    using traits_type = Traits;
    using value_type = CharT;
    using size_type = std::size_t;

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    using const_iterator = const_pointer;
    using iterator = const_iterator;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using reverse_iterator = const_reverse_iterator;

    using difference_type = std::ptrdiff_t;

public:
    static constexpr size_type npos{ size_type(-1) };

public:  /// 构造函数
    constexpr basic_string_view() noexcept = default;
    constexpr basic_string_view(const basic_string_view& rhs) noexcept = default;
    
    constexpr basic_string_view(const value_type* str)
        : str_(str), size_(traits_type::length(str))
    {}
    constexpr basic_string_view(const value_type* str, size_type len)
        : str_(str), size_(len)
    {}

    template<typename Allocator>
    basic_string_view(const std::basic_string<CharT, Traits, Allocator>& str) noexcept
        : str_(str.data()), size_(str.length())
    {}

    basic_string_view& operator=(const basic_string_view& rhs) noexcept = default;

public:  /// 迭代器相关函数
    constexpr const_iterator begin() const noexcept
    {
        return str_;
    }
    constexpr const_iterator cbegin() const noexcept
    {
        return str_;
    }
    constexpr const_iterator end() const noexcept
    {
        return str_ + size_;
    }
    constexpr const_iterator cend() const noexcept
    {
        return str_ + size_;
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }
    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

public:  /// 容量相关
    constexpr size_type size() const noexcept
    {
        return size_;
    }
    constexpr size_type length() const noexcept
    {
        return size_;
    }
    constexpr size_type max_size() const noexcept
    {
        return size_;
    }
    constexpr bool empty() const noexcept
    {
        return size_ == 0;
    }

public:  /// 访问
    constexpr const_reference operator[](size_type pos) const noexcept
    {
        return str_[pos];
    }
    constexpr const_reference at(size_t pos) const
    {
        return pos >= size_ ? (throw std::out_of_range("string_view::at"), str_[0]) : str_[pos];
    }

    constexpr const_reference front() const
    {
        return str_[0];
    }
    constexpr const_reference back() const
    {
        return str_[size_ - 1];
    }
    constexpr const_pointer data() const noexcept
    {
        return str_;
    }

public:  /// 修改
    CONSTEXPR void remove_prefix(size_type n)
    {
        if (n > size_) {
            n = size_;
        }
        str_ += n;
        size_ -= n;
    }

    CONSTEXPR void remove_suffix(size_type n)
    {
        if (n > size_) {
            n = size_;
        }
        size_ -= n;
    }

    CONSTEXPR void swap(basic_string_view& s) noexcept
    {
        std::swap(str_, s.str_);
        std::swap(size_, s.size_);
    }

public:  /// 转换 -> std::string
    template<typename Allocator>
    explicit operator std::basic_string<CharT, Traits, Allocator>() const
    {
        return std::basic_string<CharT, Traits, Allocator>(begin(), end());
    }

    template<typename Allocator = std::allocator<CharT>>
    std::basic_string<CharT, Traits, Allocator> to_string(const Allocator& alloc = Allocator()) const
    {
        return std::basic_string<CharT, Traits, Allocator>(begin(), end(), alloc);
    }

public:  /// 修改
    size_t copy(value_type* s, size_type n, size_type pos = 0) const
    {
        if (pos > size()) {
            throw std::out_of_range("string_view::copy");
        }
        size_type len = std::min(n, size_ - pos);
        traits_type::copy(s, data() + pos, len);
        return len;
    }

    CONSTEXPR basic_string_view substr(size_type pos, size_type n = npos) const
    {
        if (pos > size()) {
            throw std::out_of_range("string_view::substr");
        }
        return basic_string_view(data() + pos, std::min(n, size() - pos));
    }

public:  /// 比较
    /**
     * @brief   若 this 小于 s 则为负值.
     *          若 this 等于 s 则为 ​0​.
     *          若 this 大于 s 则为正值.
     */
    CONSTEXPR int compare(basic_string_view s) const noexcept
    {
        const int cmp = traits_type::compare(str_, s.str_, std::min(size_, s.size_));
        return cmp != 0 ? cmp : (size_ == s.size_ ? 0 : size_ < s.size_ ? -1 : 1);
    }

    constexpr int compare(size_type pos, size_type n, basic_string_view s) const noexcept
    {
        return substr(pos, n).compare(s);
    }
    constexpr int compare(size_type pos1, size_type n1, basic_string_view s,
        size_type pos2, size_type n2) const
    {
        return substr(pos1, n1).compare(s.substr(pos2, n2));
    }

    constexpr int compare(const value_type* s) const
    {
        return compare(basic_string_view(s));
    }
    constexpr int compare(size_type pos, size_type n, const value_type* s) const
    {
        return substr(pos, n).compare(basic_string_view(s));
    }
    constexpr int compare(size_type pos1, size_type n1, const value_type* s, size_type n2) const
    {
        return substr(pos1, n1).compare(basic_string_view(s, n2));
    }

public:
    constexpr bool starts_with(value_type c) const noexcept
    {
        return !empty() && traits_type::eq(c, front());
    }
    constexpr bool starts_with(basic_string_view s) const noexcept
    {
        return size_ >= s.size_ && traits_type::compare(str_, s.str_, s.size_) == 0;
    }
    constexpr bool ends_with(value_type c) const noexcept
    {
        return !empty() && traits_type::eq(c, back());
    }
    constexpr bool ends_with(basic_string_view s) const noexcept
    {
        return size_ >= s.size_ && traits_type::compare(str_ + size_ - s.size_, s.str_, s.size_) == 0;
    }

public:  /// 查找 find
    CONSTEXPR size_type find(basic_string_view s, size_type pos = 0) const noexcept
    {
        if (pos > size()) {
            return npos;
        }
        if (s.empty()) {
            return pos;
        }
        if (s.size() > size() - pos) {
            return npos;
        }

        const value_type* cur = str_ + pos;
        const value_type* last = cend() - s.size() + 1;
        for (; cur != last; ++cur) {
            cur = traits_type::find(cur, last - cur, s[0]);
            if (nullptr == cur) {
                return npos;
            }
            if (traits_type::compare(cur, s.cbegin(), s.size()) == 0) {
                return cur - str_;
            }
        }
        return npos;
    }
    CONSTEXPR size_type find(value_type c, size_type pos = 0) const noexcept
    {
        if (pos > size()) {
            return npos;
        }
        const value_type* res = traits_type::find(str_ + pos, size_ - pos, c);
        if (nullptr == res) {
            return npos;
        }
        return res - str_;
    }
    constexpr size_type find(const value_type* s, size_type pos = 0) const noexcept
    {
        return find(basic_string_view(s), pos);
    }
    // n = size(s)
    constexpr size_type find(const value_type* s, size_type pos, size_type n) const noexcept
    {
        return find(basic_string_view(s, n), pos);
    }

public:  /// 查找 rfind
    CONSTEXPR size_type rfind(basic_string_view s, size_type pos = npos) const noexcept
    {
        if (size_ < s.size()) {
            return npos;
        }
        if (pos > size_ - s.size()) {
            pos = size_ - s.size();
        }
        if (s.empty()) {
            return pos;
        }
        const value_type* cur = str_ + pos;
        for (; cur != str_; --cur) {
            if (traits_type::compare(cur, s.cbegin(), s.size()) == 0) {
                return cur - str_;
            }
        }
        return npos;
    }
    constexpr size_type rfind(value_type c, size_type pos = npos) const noexcept
    {
        return rfind(basic_string_view(&c, 1), pos);
    }
    constexpr size_type rfind(const value_type* s, size_type pos = npos) const noexcept
    {
        return rfind(basic_string_view(s), pos);
    }
    constexpr size_type rfind(const value_type* s, size_type pos, size_type n) const noexcept
    {
        return rfind(basic_string_view(s, n), pos);
    }

public:  /// 查找 find_first_of
    CONSTEXPR size_type find_first_of(basic_string_view s, size_type pos = 0) const noexcept
    {
        if (pos > size() || s.empty()) {
            return npos;
        }
        const_iterator iter = std::find_first_of(cbegin() + pos, cend(), s.cbegin(), s.cend(), traits_type::eq);
        return iter == cend() ? npos : std::distance(cbegin(), iter);
    }
    constexpr size_type find_first_of(value_type c, size_type pos = 0) const noexcept
    {
        return find(c, pos);
    }
    constexpr size_type find_first_of(const value_type* s, size_type pos = 0) const noexcept
    {
        return find_first_of(basic_string_view(s), pos);
    }
    constexpr size_type find_first_of(const value_type* s, size_type pos, size_type n) const noexcept
    {
        return find_first_of(basic_string_view(s, n), pos);
    }

public:  /// 查找 find_last_of
    CONSTEXPR size_type find_last_of(basic_string_view s, size_type pos = npos) const noexcept
    {
        if (s.empty()) {
            return npos;
        }
        if (pos >= size_) {
            pos = 0;
        }
        else {
            pos = size_ - pos - 1;
        }
        const_reverse_iterator iter = std::find_first_of(crbegin() + pos, crend(), s.cbegin(), s.cend(), traits_type::eq);
        return iter == crend() ? npos : reverse_distance(crbegin(), iter);
    }
    constexpr size_type find_last_of(value_type c, size_type pos = npos) const noexcept
    {
        return find_last_of(basic_string_view(&c, 1), pos);
    }
    constexpr size_type find_last_of(const value_type* s, size_type pos = npos) const noexcept
    {
        return find_last_of(basic_string_view(s), pos);
    }
    constexpr size_type find_last_of(const value_type* s, size_type pos, size_type n) const noexcept
    {
        return find_last_of(basic_string_view(s, n), pos);
    }

public:  /// 查找 find_first_not_of
    CONSTEXPR size_type find_first_not_of(basic_string_view s, size_type pos = 0) const noexcept
    {
        if (pos >= size_) {
            return npos;
        }
        if (s.empty()) {
            return pos;
        }
        const_iterator iter = find_not_of(cbegin() + pos, cend(), s);
        return iter == cend() ? npos : std::distance(cbegin(), iter);
    }
    constexpr size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept
    {
        return find_first_not_of(basic_string_view(&c, 1), pos);
    }
    constexpr size_type find_first_not_of(const value_type* s, size_type pos = 0) const noexcept
    {
        return find_first_not_of(basic_string_view(s), pos);
    }
    constexpr size_type find_first_not_of(const value_type* s, size_type pos, size_type n) const noexcept
    {
        return find_first_not_of(basic_string_view(s, n), pos);
    }

public:  /// 查找 find_last_not_of
    CONSTEXPR size_type find_last_not_of(basic_string_view s, size_type pos = npos) const noexcept
    {
        if (pos >= size_) {
            pos = size_ - 1;
        }
        if (s.empty()) {
            return pos;
        }
        pos = size_ - pos - 1;
        const_reverse_iterator iter = find_not_of(crbegin() + pos, crend(), s);
        return iter == crend() ? npos : reverse_distance(crbegin(), iter);
    }
    constexpr size_type find_last_not_of(value_type c, size_type pos = 0) const noexcept
    {
        return find_last_not_of(basic_string_view(&c, 1), pos);
    }
    constexpr size_type find_last_not_of(const value_type* s, size_type pos = 0) const noexcept
    {
        return find_last_not_of(basic_string_view(s), pos);
    }
    constexpr size_type find_last_not_of(const value_type* s, size_type pos, size_type n) const noexcept
    {
        return find_last_not_of(basic_string_view(s, n), pos);
    }

private:
    template<typename RevIter>
    size_type reverse_distance(RevIter first, RevIter last) const noexcept
    {
        return size_ - 1 - std::distance(first, last);
    }

    template<typename Iterator>
    Iterator find_not_of(Iterator first, Iterator last, basic_string_view s) const noexcept
    {
        for (; first != last; ++first) {
            if (0 == traits_type::find(s.cbegin(), s.size(), *first)) {
                return first;
            }
        }
        return last;
    }
private:
    const value_type* str_{ nullptr };
    size_type size_{ 0 };
};

template<typename CharT, typename Traits>
constexpr typename basic_string_view<CharT, Traits>::size_type basic_string_view<CharT, Traits>::npos;

/// 比较运算符
template<typename CharT, typename Traits>
inline CONSTEXPR bool operator==(basic_string_view<CharT, Traits> l, basic_string_view<CharT, Traits> r) noexcept
{
    if (l.size() != r.size()) {
        return false;
    }
    return l.compare(r) == 0;
}

template<typename CharT, typename Traits>
inline CONSTEXPR bool operator!=(basic_string_view<CharT, Traits> l, basic_string_view<CharT, Traits> r) noexcept
{
    if (l.size() != r.size()) {
        return true;
    }
    return l.compare(r) != 0;
}

template<typename CharT, typename Traits>
inline constexpr bool operator<(basic_string_view<CharT, Traits> l, basic_string_view<CharT, Traits> r) noexcept
{
    return l.compare(r) < 0;
}

template<typename CharT, typename Traits>
inline constexpr bool operator>(basic_string_view<CharT, Traits> l, basic_string_view<CharT, Traits> r) noexcept
{
    return l.compare(r) > 0;
}

template<typename CharT, typename Traits>
inline constexpr bool operator<=(basic_string_view<CharT, Traits> l, basic_string_view<CharT, Traits> r) noexcept
{
    return l.compare(r) <= 0;
}

template<typename CharT, typename Traits>
inline constexpr bool operator>=(basic_string_view<CharT, Traits> l, basic_string_view<CharT, Traits> r) noexcept
{
    return l.compare(r) >= 0;
}


template<typename CharT, typename Traits, typename Allocator>
inline constexpr bool operator==(basic_string_view<CharT, Traits> l, const std::basic_string<CharT, Traits, Allocator>& r) noexcept
{
    return l == basic_string_view<CharT, Traits>(r);
}
template<typename CharT, typename Traits, typename Allocator>
inline constexpr bool operator==(const std::basic_string<CharT, Traits, Allocator>& l, basic_string_view<CharT, Traits> r) noexcept
{
    return r == basic_string_view<CharT, Traits>(l);
}
template<typename CharT, typename Traits>
inline constexpr bool operator==(basic_string_view<CharT, Traits> l, const CharT* r) noexcept
{
    return l == basic_string_view<CharT, Traits>(r);
}
template<typename CharT, typename Traits>
inline constexpr bool operator==(const CharT* l, basic_string_view<CharT, Traits> r) noexcept
{
    return r == basic_string_view<CharT, Traits>(l);
}


template<typename CharT, typename Traits, typename Allocator>
inline constexpr bool operator!=(basic_string_view<CharT, Traits> l, const std::basic_string<CharT, Traits, Allocator>& r) noexcept
{
    return l != basic_string_view<CharT, Traits>(r);
}
template<typename CharT, typename Traits, typename Allocator>
inline constexpr bool operator!=(const std::basic_string<CharT, Traits, Allocator>& l, basic_string_view<CharT, Traits> r) noexcept
{
    return r != basic_string_view<CharT, Traits>(l);
}
template<typename CharT, typename Traits>
inline constexpr bool operator!=(basic_string_view<CharT, Traits> l, const CharT* r) noexcept
{
    return l != basic_string_view<CharT, Traits>(r);
}
template<typename CharT, typename Traits>
inline constexpr bool operator!=(const CharT* l, basic_string_view<CharT, Traits> r) noexcept
{
    return r != basic_string_view<CharT, Traits>(l);
}


template<typename CharT, typename Traits, typename Allocator>
inline constexpr bool operator<(basic_string_view<CharT, Traits> l, const std::basic_string<CharT, Traits, Allocator>& r) noexcept
{
    return l < basic_string_view<CharT, Traits>(r);
}
template<typename CharT, typename Traits, typename Allocator>
inline constexpr bool operator<(const std::basic_string<CharT, Traits, Allocator>& l, basic_string_view<CharT, Traits> r) noexcept
{
    return basic_string_view<CharT, Traits>(l) < r;
}
template<typename CharT, typename Traits>
inline constexpr bool operator<(basic_string_view<CharT, Traits> l, const CharT* r) noexcept
{
    return l < basic_string_view<CharT, Traits>(r);
}
template<typename CharT, typename Traits>
inline constexpr bool operator<(const CharT* l, basic_string_view<CharT, Traits> r) noexcept
{
    return basic_string_view<CharT, Traits>(l) < r;
}


template<typename CharT, typename Traits, typename Allocator>
inline constexpr bool operator>(basic_string_view<CharT, Traits> l, const std::basic_string<CharT, Traits, Allocator>& r) noexcept
{
    return l > basic_string_view<CharT, Traits>(r);
}
template<typename CharT, typename Traits, typename Allocator>
inline constexpr bool operator>(const std::basic_string<CharT, Traits, Allocator>& l, basic_string_view<CharT, Traits> r) noexcept
{
    return basic_string_view<CharT, Traits>(l) > r;
}
template<typename CharT, typename Traits>
inline constexpr bool operator>(basic_string_view<CharT, Traits> l, const CharT* r) noexcept
{
    return l > basic_string_view<CharT, Traits>(r);
}
template<typename CharT, typename Traits>
inline constexpr bool operator>(const CharT* l, basic_string_view<CharT, Traits> r) noexcept
{
    return basic_string_view<CharT, Traits>(l) > r;
}


template<typename CharT, typename Traits, typename Allocator>
inline constexpr bool operator<=(basic_string_view<CharT, Traits> l, const std::basic_string<CharT, Traits, Allocator>& r) noexcept
{
    return l <= basic_string_view<CharT, Traits>(r);
}
template<typename CharT, typename Traits, typename Allocator>
inline constexpr bool operator<=(const std::basic_string<CharT, Traits, Allocator>& l, basic_string_view<CharT, Traits> r) noexcept
{
    return basic_string_view<CharT, Traits>(l) <= r;
}
template<typename CharT, typename Traits>
inline constexpr bool operator<=(basic_string_view<CharT, Traits> l, const CharT* r) noexcept
{
    return l <= basic_string_view<CharT, Traits>(r);
}
template<typename CharT, typename Traits>
inline constexpr bool operator<=(const CharT* l, basic_string_view<CharT, Traits> r) noexcept
{
    return basic_string_view<CharT, Traits>(l) <= r;
}


template<typename CharT, typename Traits, typename Allocator>
inline constexpr bool operator>=(basic_string_view<CharT, Traits> l, const std::basic_string<CharT, Traits, Allocator>& r) noexcept
{
    return l >= basic_string_view<CharT, Traits>(r);
}
template<typename CharT, typename Traits, typename Allocator>
inline constexpr bool operator>=(const std::basic_string<CharT, Traits, Allocator>& l, basic_string_view<CharT, Traits> r) noexcept
{
    return basic_string_view<CharT, Traits>(l) >= r;
}
template<typename CharT, typename Traits>
inline constexpr bool operator>=(basic_string_view<CharT, Traits> l, const CharT* r) noexcept
{
    return l >= basic_string_view<CharT, Traits>(r);
}
template<typename CharT, typename Traits>
inline constexpr bool operator>=(const CharT* l, basic_string_view<CharT, Traits> r) noexcept
{
    return basic_string_view<CharT, Traits>(l) >= r;
}

using string_view = basic_string_view<char, std::char_traits<char>>;

BASE_END_NAMESPACE
