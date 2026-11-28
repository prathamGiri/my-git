#include "mygit/zlib.hpp"

#include <zlib.h>
#include <string>
#include <stdexcept>

std::string zcompress(const std::string& in, int level) {
    uLongf outLen = compressBound(in.size());
    std::string out(outLen, '\0');
    int rc = compress2(
        reinterpret_cast<Bytef*>(out.data()),
        &outLen,
        reinterpret_cast<const Bytef*>(in.data()),
        in.size(), 
        level
    );
    if (rc != Z_OK) throw std::runtime_error("compress2 failed");
    out.resize(outLen);
    return out;
}

std::string zdecompress(const std::string& in) {
    z_stream zs{};
    if (inflateInit(&zs) != Z_OK) throw std::runtime_error("inflateInit failed");

    zs.next_in  = reinterpret_cast<Bytef*>(const_cast<char*>(in.data()));
    zs.avail_in = static_cast<uInt>(in.size());

    std::string out;
    char buf[16384];
    int rc;
    do {
        zs.next_out  = reinterpret_cast<Bytef*>(buf);
        zs.avail_out = sizeof(buf);
        rc = inflate(&zs, Z_NO_FLUSH);
        if (rc != Z_OK && rc != Z_STREAM_END) {
            inflateEnd(&zs);
            throw std::runtime_error("inflate failed: corrupt or truncated data");
        }
        out.append(buf, sizeof(buf) - zs.avail_out);
    } while (rc != Z_STREAM_END);

    inflateEnd(&zs);
    return out;
}