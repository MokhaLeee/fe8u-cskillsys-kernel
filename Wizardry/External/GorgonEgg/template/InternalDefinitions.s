
.macro SET_FUNC name, value
  .global \name
  .type   \name, function
  .set    \name, \value
.endm

.macro SET_DATA name, value
  .global \name
  .type   \name, object
  .set    \name, \value
.endm

.include "GeneratedASMDefinitions.s"

SET_FUNC GetCursorQuadrant,     (0x0808BBCC+1)
SET_FUNC GetWindowQuadrant,     (0x0808BBAC+1)

SET_DATA gpStatScreenPageBg1Map,     0x0200422C

SET_DATA sPlayerInterfaceConfigLut,  0x08A01828
