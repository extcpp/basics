#!/usr/bin/python3
from pathlib import Path
import obi.util.logging_helper as lh

logger = lh.init_logging(Path(__name__).stem)
for handler in logger.handlers:
    lh.add_obi_formatter_short(handler)
