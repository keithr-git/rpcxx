//
//    Copyright 2008, 2009 Keith Reynolds.
//
//    This file is part of RPC++.
//
//    RPC++ is free software: you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation,
//    either version 2.1 of the License, or (at your option) any
//    later version.
//
//    RPC++ is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty
//    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//    See the GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General
//    Public License along with RPC++.  If not, see
//    <http://www.gnu.org/licenses/>.
//
#ifndef TEST1_TYPES_HPP
#define TEST1_TYPES_HPP

#include <sys/stat.h>

#include <stdint.h>

#include <rpc++/buffer.hpp>
#include <rpc++/marshal.hpp>

struct Time
{
    uint32_t    time_seconds;
    uint32_t    time_nanoseconds;
};

struct Stat
{
    uint64_t    stat_dev;
    uint64_t    stat_ino;
    uint32_t    stat_mode;
    uint32_t    stat_nlink;
    uint64_t    stat_uid;
    uint64_t    stat_gid;
    uint64_t    stat_rdev;
    uint64_t    stat_size;
    uint32_t    stat_blksize;
    uint64_t    stat_blocks;
    Time        stat_atime;
    Time        stat_mtime;
    Time        stat_ctime;
};

namespace RPCXX
{
    namespace marshal
    {
        template<>
        struct net_type<struct stat>
        {
            typedef Stat        type;
        };
    }; // namespace marshal

    static NetBuffer& operator>>(NetBuffer& buffer, struct stat& value)
    {
        const Stat& net_value = buffer.head<Stat>();

        marshal::from_net(value.st_dev, net_value.stat_dev);
        marshal::from_net(value.st_ino, net_value.stat_ino);
        marshal::from_net(value.st_mode, net_value.stat_mode);
        marshal::from_net(value.st_nlink, net_value.stat_nlink);
        marshal::from_net(value.st_uid, net_value.stat_uid);
        marshal::from_net(value.st_gid, net_value.stat_gid);
        marshal::from_net(value.st_rdev, net_value.stat_rdev);
        marshal::from_net(value.st_size, net_value.stat_size);
        marshal::from_net(value.st_blksize, net_value.stat_blksize);
        marshal::from_net(value.st_blocks, net_value.stat_blocks);
        marshal::from_net(value.st_atime, net_value.stat_atime.time_seconds);
        marshal::from_net(value.st_atim.tv_nsec,
                net_value.stat_atime.time_nanoseconds);
        marshal::from_net(value.st_mtime, net_value.stat_mtime.time_seconds);
        marshal::from_net(value.st_mtim.tv_nsec,
                net_value.stat_mtime.time_nanoseconds);
        marshal::from_net(value.st_ctime, net_value.stat_ctime.time_seconds);
        marshal::from_net(value.st_ctim.tv_nsec,
                net_value.stat_ctime.time_nanoseconds);

        return buffer;
    }

    static NetBuffer& operator<<(NetBuffer& buffer, const struct stat& value)
    {
        Stat& net_value = buffer.tail<Stat>();

        marshal::to_net(value.st_dev, net_value.stat_dev);
        marshal::to_net(value.st_ino, net_value.stat_ino);
        marshal::to_net(value.st_mode, net_value.stat_mode);
        marshal::to_net(value.st_nlink, net_value.stat_nlink);
        marshal::to_net(value.st_uid, net_value.stat_uid);
        marshal::to_net(value.st_gid, net_value.stat_gid);
        marshal::to_net(value.st_rdev, net_value.stat_rdev);
        marshal::to_net(value.st_size, net_value.stat_size);
        marshal::to_net(value.st_blksize, net_value.stat_blksize);
        marshal::to_net(value.st_blocks, net_value.stat_blocks);
        marshal::to_net(value.st_atime, net_value.stat_atime.time_seconds);
        marshal::to_net(value.st_atim.tv_nsec,
                net_value.stat_atime.time_nanoseconds);
        marshal::to_net(value.st_mtime, net_value.stat_mtime.time_seconds);
        marshal::to_net(value.st_mtim.tv_nsec,
                net_value.stat_mtime.time_nanoseconds);
        marshal::to_net(value.st_ctime, net_value.stat_ctime.time_seconds);
        marshal::to_net(value.st_ctim.tv_nsec,
                net_value.stat_ctime.time_nanoseconds);

        return buffer;
    }
}; // namespace RPCXX

#endif /* TEST1_TYPES_HPP */
