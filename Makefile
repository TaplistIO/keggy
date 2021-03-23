BOARD := esp32:esp32:m5stick-c
SRCS := keggy.ino
EXTRA_BUILD_FLAGS:= -v
DEVICE_PORT := /dev/cu.usbserial-4D5A4113AB

compile: $(SRCS)
	arduino-cli compile \
		-b $(BOARD) \
		$(EXTRA_BUILD_FLAGS) \
		$(SRCS)

upload:
	arduino-cli upload \
		-v \
		-p $(DEVICE_PORT) \
		-b $(BOARD)

.PHONY: compile upload

