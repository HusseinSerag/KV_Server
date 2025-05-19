import logging
def setup_logger(name, filepath):
    logger = logging.getLogger(name)
    logger.setLevel(logging.INFO)
    if not logger.handlers:
        fh = logging.FileHandler(filepath)
        formatter = logging.Formatter("%(asctime)s  %(message)s")
        fh.setFormatter(formatter)
        logger.addHandler(fh)
        logger.propagate = False
    return logger