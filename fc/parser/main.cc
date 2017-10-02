//  Copyright (c) 2017-2017 The Chaos  Authors. All rights reserved.
//  Created on: 2017.9.29 Author: kerry

#include "file/file_path.h"
#include "file/file_util.h"
#include "logic/logic_comm.h"
#include "his_head.h"
#include "flw_his_file.h"
#include "proto/symbol_dynam_market.pb.h"
#include "protocol/data_packet.h"
#include <string>

const char* me = NULL;

void usage() {
    LOG_MSG("-dir {path} {out_base_dir}");
    LOG_MSG("-file {filename} {out_base_dir}");
}

int main(int agrc, char* argv[]) {
  me = argv[0];
  LOG_MSG2("%s", me);
  if (agrc < 3) {
    usage();
    return 1;
  }
  LOG_MSG2("%s",argv[1]);
  if (argv[1][0] == '-' && argv[1][1] == 'd' && argv[1][2] == 'i'
      && argv[1][3] == 'r') {
    std::string dir = argv[2];
    std::string out_dir = argv[3];
    LOG_MSG2("%s", dir.c_str());
    file::FilePath dir_path(dir);
    std::list<file::FilePath> file_list;
    file::GetDirectoryFile(dir_path, file_list);
    while (file_list.size() > 0) {
      file::FilePath file = file_list.front();
      file_list.pop_front();
      LOG_MSG2("file:%s", file.value().c_str());
      fc_data::FlwHisFile his_file(out_dir);
      his_file.OpenHisFile2Read(file.value());
    }
  }else if(argv[1][0] == '-' && argv[1][1] == 'f' && argv[1][2] == 'i'
          && argv[1][3] == 'l' && argv[1][4] == 'e'){
      std::string file = argv[2];
      std::string out_dir = argv[3];
      fc_data::FlwHisFile his_file(out_dir);
      his_file.OpenHisFile2Read(file);
   }else{
    usage();
  }
  /*std::string dir = "/root/ky/ChaosTransaction/ironman/test/ZC";
  std::string out_dir = "/home/tmt";
    file::FilePath dir_path(dir);
    std::list<file::FilePath> file_list;
    file::GetDirectoryFile(dir_path, file_list);
    while (file_list.size() > 0) {
      file::FilePath file = file_list.front();
      file_list.pop_front();
      LOG_MSG2("file:%s", file.value().c_str());
      fc_data::FlwHisFile his_file(out_dir);
      his_file.OpenHisFile2Read(file.value());
    }*/
  
/*
    chaos_data::SymbolDynamMarket dynam_markert;
    dynam_markert.set_current_time(1506827281);
    dynam_markert.set_open_price(12351);
    dynam_markert.set_high_price(12341);
    dynam_markert.set_low_price(3131);
    dynam_markert.set_new_price(12312);
    dynam_markert.set_volume(887712);
    dynam_markert.set_amount(1541);
    dynam_markert.set_inner_vol(241);
    dynam_markert.set_tick_count(6666);

    //----->
    dynam_markert.add_buy_price(231361);
    dynam_markert.add_buy_vol(321261);
    dynam_markert.add_sell_price(22361);
    dynam_markert.add_sell_vol(33261);

    
    //----->
    dynam_markert.add_buy_price(231362);
    dynam_markert.add_buy_vol(321262);
    dynam_markert.add_sell_price(22362);
    dynam_markert.add_sell_vol(33262);
    //----->
    dynam_markert.add_buy_price(231363);
    dynam_markert.add_buy_vol(321263);
    dynam_markert.add_sell_price(22363);
    dynam_markert.add_sell_vol(33263);
    //----->
    dynam_markert.add_buy_price(231364);
    dynam_markert.add_buy_vol(321264);
    dynam_markert.add_sell_price(22364);
    dynam_markert.add_sell_vol(33264);
    //----->
    dynam_markert.add_buy_price(231365);
    dynam_markert.add_buy_vol(321265);
    dynam_markert.add_sell_price(22365);
    dynam_markert.add_sell_vol(33265);
    std::string in_data;
    bool r = dynam_markert.SerializeToString(&in_data);
    int32 packet_length = in_data.length() + sizeof(int16);
    packet::DataOutPacket out(true, packet_length);
    out.Write16(packet_length);
    out.WriteData(in_data.c_str(),in_data.length());
*/
}
