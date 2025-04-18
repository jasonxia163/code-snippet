#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

int width_ = 0;
int height_ = 0;

int ReadUe(const uint8_t* buf, int* bit_offset, size_t size) {
  int leading_zero_bits = 0;
  int old_offset = *bit_offset;
  while (*bit_offset < 8 * size) {
    if ((buf[*bit_offset / 8] & (1 << (7 - (*bit_offset % 8)))) != 0) {
      break;
    }
    leading_zero_bits++;
    (*bit_offset)++;
  }
  (*bit_offset)++;

  int value = (1 << leading_zero_bits) - 1;
  for (int i = 0; i < leading_zero_bits; i++) {
    value += ((buf[*bit_offset / 8] >> (7 - (*bit_offset % 8))) & 1)
             << (leading_zero_bits - i - 1);
    (*bit_offset)++;
  }
  printf("ReadUe: %d, offset: %d->%d\n", value, old_offset, *bit_offset);
  return value;
}

int ReadBit(const uint8_t* buf, int* bit_offset) {
  int old_offset = *bit_offset;
  int bit = (buf[*bit_offset / 8] >> (7 - (*bit_offset % 8))) & 1;
  (*bit_offset)++;
  printf("ReadBit: %d, offset: %d->%d\n", bit, old_offset, *bit_offset);
  return bit;
}

void ParseSps(const uint8_t* sps, size_t size) {
  int bit_offset = 0;
  int profile_idc = sps[bit_offset / 8];
  bit_offset += 8;
  bit_offset += 16;  // 跳过 constraints_flags 和 level_idc
  // int seq_parameter_set_id = ReadUe(sps, &bit_offset, size);
  int chroma_format_idc = 1;

  if (profile_idc == 100 || profile_idc == 110 || profile_idc == 122 ||
      profile_idc == 244 || profile_idc == 44 || profile_idc == 83 ||
      profile_idc == 86 || profile_idc == 118 || profile_idc == 128) {
    chroma_format_idc = ReadUe(sps, &bit_offset, size);
    if (chroma_format_idc == 3)
      ReadBit(sps, &bit_offset);      // separate_colour_plane_flag
    ReadUe(sps, &bit_offset, size);   // bit_depth_luma_minus8
    ReadUe(sps, &bit_offset, size);   // bit_depth_chroma_minus8
    ReadBit(sps, &bit_offset);        // qpprime_y_zero_transform_bypass_flag
    if (ReadBit(sps, &bit_offset)) {  // seq_scaling_matrix_present_flag
      for (int i = 0; i < ((chroma_format_idc != 3) ? 8 : 12); i++) {
        if (ReadBit(sps, &bit_offset)) {  // seq_scaling_list_present_flag
        }
      }
    }
  }

  ReadUe(sps, &bit_offset, size);  // log2_max_frame_num_minus4
  int pic_order_cnt_type = ReadUe(sps, &bit_offset, size);
  if (pic_order_cnt_type == 0) {
    ReadUe(sps, &bit_offset, size);  // log2_max_pic_order_cnt_lsb_minus4
  } else if (pic_order_cnt_type == 1) {
    ReadBit(sps, &bit_offset);       // delta_pic_order_always_zero_flag
    ReadUe(sps, &bit_offset, size);  // offset_for_non_ref_pic
    ReadUe(sps, &bit_offset, size);  // offset_for_top_to_bottom_field
    int num_ref_frames_in_pic_order_cnt_cycle = ReadUe(sps, &bit_offset, size);
    for (int i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++) {
      ReadUe(sps, &bit_offset, size);  // offset_for_ref_frame[i]
    }
  }
  ReadUe(sps, &bit_offset, size);  // max_num_ref_frames
  ReadBit(sps, &bit_offset);       // gaps_in_frame_num_value_allowed_flag
  int pic_width_in_mbs_minus1 = ReadUe(sps, &bit_offset, size);
  int pic_height_in_map_units_minus1 = ReadUe(sps, &bit_offset, size);
  int frame_mbs_only_flag = ReadBit(sps, &bit_offset);
  if (!frame_mbs_only_flag)
    ReadBit(sps, &bit_offset);  // mb_adaptive_frame_field_flag
  ReadBit(sps, &bit_offset);    // direct_8x8_inference_flag

  int frame_cropping_flag = ReadBit(sps, &bit_offset);
  int frame_crop_left_offset = 0, frame_crop_right_offset = 0;
  int frame_crop_top_offset = 0, frame_crop_bottom_offset = 0;

  if (frame_cropping_flag) {
    frame_crop_left_offset = ReadUe(sps, &bit_offset, size);
    frame_crop_right_offset = ReadUe(sps, &bit_offset, size);
    frame_crop_top_offset = ReadUe(sps, &bit_offset, size);
    frame_crop_bottom_offset = ReadUe(sps, &bit_offset, size);
  }

  width_ = (pic_width_in_mbs_minus1 + 1) * 16 -
           (frame_crop_left_offset + frame_crop_right_offset) * 2;
  height_ =
      (pic_height_in_map_units_minus1 + 1) * 16 * (2 - frame_mbs_only_flag) -
      (frame_crop_top_offset + frame_crop_bottom_offset) * 2;
}

int main() {
  // const uint8_t sps_data[] = {0x42, 0x00, 0x1f, 0xab, 0x40, 0x28, 0x02, 0xdc,
  // 0x80};
  const uint8_t sps_data[] = {0x42, 0xc0, 0x1f, 0x8c, 0x68, 0x05, 0x00,
                              0x5b, 0xa0, 0x1e, 0x11, 0x08, 0xd4};
  const size_t sps_size = sizeof(sps_data) / sizeof(sps_data[0]);
  ParseSps(sps_data, sps_size);
  printf("max_off     set = %d\n", sps_size * 8);
  return 0;
}
