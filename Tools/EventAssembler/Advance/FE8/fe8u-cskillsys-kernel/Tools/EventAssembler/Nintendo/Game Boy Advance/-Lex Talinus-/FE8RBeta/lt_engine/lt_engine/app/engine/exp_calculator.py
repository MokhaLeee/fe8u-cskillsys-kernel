import math

from enum import Enum

class ExpCalcType(Enum):
  STANDARD = 'standard'
  GOMPERTZ = 'gompertz'

class ExpCalculator():
  @classmethod
  def classical_curve_calculator(cls, level_diff, exp_offset, exp_curve, exp_magnitude):
    adjusted_level_diff = level_diff + exp_offset
    exp_gained = math.exp(adjusted_level_diff * exp_curve)
    total_exp_gained = exp_magnitude * exp_gained
    return total_exp_gained

  @classmethod
  def gompertz_curve_calculator(cls, level_diff, max_exp, min_exp, slope, zero_diff_exp):
    base = min_exp
    max_exp = max_exp + 1 # because we use floor
    magnitude = max_exp - min_exp
    offset = ExpCalculator.gompertz_offset_calculator(magnitude, min_exp, slope, zero_diff_exp)
    return base + magnitude * math.exp(-1 * math.exp(-slope * (level_diff - offset)))

  @classmethod
  def gompertz_offset_calculator(cls, magnitude, min_exp, slope, intercept):
    return 1.0 / slope * math.log(-1 * math.log((intercept - min_exp) / magnitude))

if __name__ == "__main__":
  # generate some curves
  max_exp = 20
  min_exp = 1
  slope = 0.5
  zero_diff_exp = 10
  for diff in range(-10, 10):
    exp = ExpCalculator.gompertz_curve_calculator(diff, max_exp, min_exp, slope, zero_diff_exp)
    print("%d,\t%d" % (diff, exp * 3))