import atexit
import ctypes
import ctypes.util
import functools
import logging
import os
import signal
import socket
import subprocess
import sys

DEBUG = bool(os.getenv("DEBUG", False))
logger = logging.getLogger(__name__)


class Singleton(type):
    _instances = {}

    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super(Singleton, cls).__call__(*args, **kwargs)
        return cls._instances[cls]


class Server(metaclass=Singleton):
    def __init__(self, port: int | None = None):
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self._socket.bind(("", 0))
        self._socket.listen()
        atexit.register(self.close)
        signal.signal(signal.SIGTERM, self.close)
        signal.signal(signal.SIGABRT, self.close)

        self._client = None

        self._port = self._socket.getsockname()[1]
        logger.info(f"Started stechec spectator on port {port}.")

    @property
    def port(self):
        return self._port

    def accept(self):
        self._client, (host, port) = self._socket.accept()
        logger.info(
            f"Successfully established connection with viewer at {host}:{port}."
        )

    def send_dump(self):
        assert self._client is not None, "No connected client."

        dump = libprologin().dump_state_json()
        self._client.sendall(dump)

        logger.info(f"{len(dump)} bytes sent to viewer.")

    def wait_for_next(self):
        assert self._client is not None, "No connected viewer."

        expected = b"NEXT"
        logger.debug("Waiting for response from viewer.")

        msg = b""
        try:
            while msg != expected:
                data = self._client.recv(1024)
                if not data:
                    raise ConnectionResetError

                # Godot sends a bunch of null bytes and an EOT
                # but the latter is not necessarily at then end...
                msg += data.replace(b"\x00", b"").replace(b"\x04", b"")
        except ConnectionResetError:
            logger.error("Connection interrupted by the viewer. Exiting.")
            sys.exit(1)

        logger.debug("Response received.")

    def close(self):
        logger.info("Terminating the connection.")
        if self._client is not None:
            self._client.close()
        self._socket.close()


@functools.cache
def libprologin() -> ctypes.CDLL:
    try:
        lib = ctypes.CDLL("libprologin2024.so")
    except OSError:
        logger.critical("Could not find dynamic library prologin2024.")
        sys.exit(1)

    logger.info(f"Found library {lib._name}.")
    lib.dump_state_json.restype = ctypes.c_char_p

    return lib


def launch_viewer(port: int):
    if DEBUG:
        logger.info(
            f"Debug mode enabled. Please launch the viewer manually (./path/to/viewer -socket={port})"
        )
        return

    logger.info("Starting viewer.")
    subprocess.Popen(
        [
            "/run/current-system/sw/bin/prologin2024",
            f"-socket={port}",
        ]
    )


def partie_init():
    logging.basicConfig(level=logging.DEBUG if DEBUG else logging.INFO)

    server = Server()
    launch_viewer(server.port)
    server.accept()

    server.send_dump()
    server.wait_for_next()


def jouer_tour():
    server = Server()
    server.send_dump()
    server.wait_for_next()


def partie_fin():
    server = Server()
    server.send_dump()
    server.wait_for_next()
    server.close()
