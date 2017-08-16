# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: datayes_mktfut.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='datayes_mktfut.proto',
  package='chaosdata',
  syntax='proto2',
  serialized_pb=_b('\n\x14\x64\x61tayes_mktfut.proto\x12\tchaosdata\"\xc4\x03\n\x06MKTFut\x12\x10\n\x06sec_id\x18\x01 \x02(\t:\x00\x12\x10\n\x06ticker\x18\x02 \x02(\t:\x00\x12\x14\n\nshort_name\x18\x03 \x02(\t:\x00\x12\x15\n\x0b\x65xchange_id\x18\x04 \x02(\t:\x00\x12\x16\n\ntrade_date\x18\x05 \x02(\x03:\x02-1\x12\x19\n\x0f\x63ontract_object\x18\x06 \x02(\t:\x00\x12\x17\n\rcontract_mark\x18\x07 \x02(\t:\x00\x12\x1b\n\x0fpre_settl_price\x18\x08 \x02(\x01:\x02-1\x12\x1b\n\x0fpre_close_price\x18\t \x02(\x01:\x02-1\x12\x16\n\nopen_price\x18\n \x02(\x01:\x02-1\x12\x1a\n\x0ehightest_price\x18\x0b \x02(\x01:\x02-1\x12\x18\n\x0clowest_price\x18\x0c \x02(\x01:\x02-1\x12\x17\n\x0bsettl_price\x18\r \x02(\x01:\x02-1\x12\x17\n\x0b\x63lose_price\x18\x0e \x02(\x01:\x02-1\x12\x18\n\x0cturnover_vol\x18\x0f \x02(\x01:\x02-1\x12\x1a\n\x0eturnover_value\x18\x10 \x02(\x01:\x02-1\x12\x14\n\x08open_int\x18\x11 \x02(\x01:\x02-1\x12\x17\n\x0bupdate_time\x18\x12 \x02(\x03:\x02-1')
)




_MKTFUT = _descriptor.Descriptor(
  name='MKTFut',
  full_name='chaosdata.MKTFut',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='sec_id', full_name='chaosdata.MKTFut.sec_id', index=0,
      number=1, type=9, cpp_type=9, label=2,
      has_default_value=True, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='ticker', full_name='chaosdata.MKTFut.ticker', index=1,
      number=2, type=9, cpp_type=9, label=2,
      has_default_value=True, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='short_name', full_name='chaosdata.MKTFut.short_name', index=2,
      number=3, type=9, cpp_type=9, label=2,
      has_default_value=True, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='exchange_id', full_name='chaosdata.MKTFut.exchange_id', index=3,
      number=4, type=9, cpp_type=9, label=2,
      has_default_value=True, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='trade_date', full_name='chaosdata.MKTFut.trade_date', index=4,
      number=5, type=3, cpp_type=2, label=2,
      has_default_value=True, default_value=-1,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='contract_object', full_name='chaosdata.MKTFut.contract_object', index=5,
      number=6, type=9, cpp_type=9, label=2,
      has_default_value=True, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='contract_mark', full_name='chaosdata.MKTFut.contract_mark', index=6,
      number=7, type=9, cpp_type=9, label=2,
      has_default_value=True, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='pre_settl_price', full_name='chaosdata.MKTFut.pre_settl_price', index=7,
      number=8, type=1, cpp_type=5, label=2,
      has_default_value=True, default_value=float(-1),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='pre_close_price', full_name='chaosdata.MKTFut.pre_close_price', index=8,
      number=9, type=1, cpp_type=5, label=2,
      has_default_value=True, default_value=float(-1),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='open_price', full_name='chaosdata.MKTFut.open_price', index=9,
      number=10, type=1, cpp_type=5, label=2,
      has_default_value=True, default_value=float(-1),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='hightest_price', full_name='chaosdata.MKTFut.hightest_price', index=10,
      number=11, type=1, cpp_type=5, label=2,
      has_default_value=True, default_value=float(-1),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='lowest_price', full_name='chaosdata.MKTFut.lowest_price', index=11,
      number=12, type=1, cpp_type=5, label=2,
      has_default_value=True, default_value=float(-1),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='settl_price', full_name='chaosdata.MKTFut.settl_price', index=12,
      number=13, type=1, cpp_type=5, label=2,
      has_default_value=True, default_value=float(-1),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='close_price', full_name='chaosdata.MKTFut.close_price', index=13,
      number=14, type=1, cpp_type=5, label=2,
      has_default_value=True, default_value=float(-1),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='turnover_vol', full_name='chaosdata.MKTFut.turnover_vol', index=14,
      number=15, type=1, cpp_type=5, label=2,
      has_default_value=True, default_value=float(-1),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='turnover_value', full_name='chaosdata.MKTFut.turnover_value', index=15,
      number=16, type=1, cpp_type=5, label=2,
      has_default_value=True, default_value=float(-1),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='open_int', full_name='chaosdata.MKTFut.open_int', index=16,
      number=17, type=1, cpp_type=5, label=2,
      has_default_value=True, default_value=float(-1),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='update_time', full_name='chaosdata.MKTFut.update_time', index=17,
      number=18, type=3, cpp_type=2, label=2,
      has_default_value=True, default_value=-1,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=36,
  serialized_end=488,
)

DESCRIPTOR.message_types_by_name['MKTFut'] = _MKTFUT
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

MKTFut = _reflection.GeneratedProtocolMessageType('MKTFut', (_message.Message,), dict(
  DESCRIPTOR = _MKTFUT,
  __module__ = 'datayes_mktfut_pb2'
  # @@protoc_insertion_point(class_scope:chaosdata.MKTFut)
  ))
_sym_db.RegisterMessage(MKTFut)


# @@protoc_insertion_point(module_scope)
