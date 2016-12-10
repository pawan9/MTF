"""autogenerated by genpy from mtf_bridge/PatchTrackers.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct

import mtf_bridge.msg

class PatchTrackers(genpy.Message):
  _md5sum = "2b617595ec9fb84f8e093a4f20db41ee"
  _type = "mtf_bridge/PatchTrackers"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """Patch[] trackers

================================================================================
MSG: mtf_bridge/Patch
Point[4] corners
Point center

================================================================================
MSG: mtf_bridge/Point
float64 x
float64 y

"""
  __slots__ = ['trackers']
  _slot_types = ['mtf_bridge/Patch[]']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       trackers

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(PatchTrackers, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.trackers is None:
        self.trackers = []
    else:
      self.trackers = []

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      length = len(self.trackers)
      buff.write(_struct_I.pack(length))
      for val1 in self.trackers:
        for val2 in val1.corners:
          _x = val2
          buff.write(_struct_2d.pack(_x.x, _x.y))
        _v1 = val1.center
        _x = _v1
        buff.write(_struct_2d.pack(_x.x, _x.y))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      if self.trackers is None:
        self.trackers = None
      end = 0
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.trackers = []
      for i in range(0, length):
        val1 = mtf_bridge.msg.Patch()
        val1.corners = []
        for i in range(0, 4):
          val2 = mtf_bridge.msg.Point()
          _x = val2
          start = end
          end += 16
          (_x.x, _x.y,) = _struct_2d.unpack(str[start:end])
          val1.corners.append(val2)
        _v2 = val1.center
        _x = _v2
        start = end
        end += 16
        (_x.x, _x.y,) = _struct_2d.unpack(str[start:end])
        self.trackers.append(val1)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      length = len(self.trackers)
      buff.write(_struct_I.pack(length))
      for val1 in self.trackers:
        for val2 in val1.corners:
          _x = val2
          buff.write(_struct_2d.pack(_x.x, _x.y))
        _v3 = val1.center
        _x = _v3
        buff.write(_struct_2d.pack(_x.x, _x.y))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      if self.trackers is None:
        self.trackers = None
      end = 0
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.trackers = []
      for i in range(0, length):
        val1 = mtf_bridge.msg.Patch()
        val1.corners = []
        for i in range(0, 4):
          val2 = mtf_bridge.msg.Point()
          _x = val2
          start = end
          end += 16
          (_x.x, _x.y,) = _struct_2d.unpack(str[start:end])
          val1.corners.append(val2)
        _v4 = val1.center
        _x = _v4
        start = end
        end += 16
        (_x.x, _x.y,) = _struct_2d.unpack(str[start:end])
        self.trackers.append(val1)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_2d = struct.Struct("<2d")
