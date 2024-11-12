image:
	@docker build -f dockerfiles/dev -t esp_idf_wifi_manager_image .
.PHONY: image

ssh-connected: image
	@docker run -it --rm \
		--device /dev/ttyUSB0:/dev/ttyUSB0 \
		-v $(PWD)/CMakeLists.txt:/esp-idf-wifi-manager/CMakeLists.txt \
		-v $(PWD)/Makefile:/esp-idf-wifi-manager/Makefile \
		-v $(PWD)/assets:/esp-idf-wifi-manager/assets \
		-v $(PWD)/dockerfiles:/esp-idf-wifi-manager/dockerfiles \
		-v $(PWD)/examples:/esp-idf-wifi-manager/examples \
		-v $(PWD)/include:/esp-idf-wifi-manager/include \
		-v $(PWD)/src:/esp-idf-wifi-manager/src \
		-v $(PWD)/test:/esp-idf-wifi-manager/test \
		-w /esp-idf-wifi-manager/ \
		esp_idf_wifi_manager_image \
		bash
.PHONY: ssh-connected

ssh: image
	@docker run -it --rm \
		-v $(PWD)/CMakeLists.txt:/esp-idf-wifi-manager/CMakeLists.txt \
		-v $(PWD)/Makefile:/esp-idf-wifi-manager/Makefile \
		-v $(PWD)/assets:/esp-idf-wifi-manager/assets \
		-v $(PWD)/dockerfiles:/esp-idf-wifi-manager/dockerfiles \
		-v $(PWD)/examples:/esp-idf-wifi-manager/examples \
		-v $(PWD)/include:/esp-idf-wifi-manager/include \
		-v $(PWD)/src:/esp-idf-wifi-manager/src \
		-v $(PWD)/test:/esp-idf-wifi-manager/test \
		-w /esp-idf-wifi-manager/ \
		esp_idf_wifi_manager_image \
		bash
.PHONY: ssh

build-basic: image
	@docker run -it --rm \
		-v $(PWD)/CMakeLists.txt:/esp-idf-wifi-manager/CMakeLists.txt \
		-v $(PWD)/assets:/esp-idf-wifi-manager/assets \
		-v $(PWD)/examples:/esp-idf-wifi-manager/examples \
		-v $(PWD)/include:/esp-idf-wifi-manager/include \
		-v $(PWD)/src:/esp-idf-wifi-manager/src \
		-w /esp-idf-wifi-manager/examples/basic \
		esp_idf_wifi_manager_image \
		sh -c " \
			. /esp-idf/export.sh && \
			idf.py set-target esp32 && \
			idf.py build \
		"
.PHONY: build-basic

upload-basic: build-basic
	@docker run -it --rm \
		--device /dev/ttyUSB0:/dev/ttyUSB0 \
		-v $(PWD)/CMakeLists.txt:/esp-idf-wifi-manager/CMakeLists.txt \
		-v $(PWD)/assets:/esp-idf-wifi-manager/assets \
		-v $(PWD)/examples:/esp-idf-wifi-manager/examples \
		-v $(PWD)/include:/esp-idf-wifi-manager/include \
		-v $(PWD)/src:/esp-idf-wifi-manager/src \
		-w /esp-idf-wifi-manager/examples/basic \
		esp_idf_wifi_manager_image \
		sh -c " \
			. /esp-idf/export.sh && \
			idf.py set-target esp32 && \
			idf.py -p /dev/ttyUSB0 flash \
		"
.PHONY: upload-basic

serial: image
	@docker run -it --rm \
		--device /dev/ttyUSB0:/dev/ttyUSB0 \
		-v $(PWD)/CMakeLists.txt:/esp-idf-wifi-manager/CMakeLists.txt \
		-v $(PWD)/assets:/esp-idf-wifi-manager/assets \
		-v $(PWD)/examples:/esp-idf-wifi-manager/examples \
		-v $(PWD)/include:/esp-idf-wifi-manager/include \
		-v $(PWD)/src:/esp-idf-wifi-manager/src \
		-w /esp-idf-wifi-manager/examples/basic \
		esp_idf_wifi_manager_image \
		sh -c " \
			. /esp-idf/export.sh && \
			idf.py -p /dev/ttyUSB0 monitor \
		"
.PHONY: serial

format: image
	@docker run --rm -it \
		-v $(PWD)/CMakeLists.txt:/esp-idf-wifi-manager/CMakeLists.txt \
		-v $(PWD)/assets:/esp-idf-wifi-manager/assets \
		-v $(PWD)/examples:/esp-idf-wifi-manager/examples \
		-v $(PWD)/include:/esp-idf-wifi-manager/include \
		-v $(PWD)/src:/esp-idf-wifi-manager/src \
		-v $(PWD)/test:/esp-idf-wifi-manager/test \
		-w /esp-idf-wifi-manager/ \
		esp_idf_wifi_manager_image \
		sh -c "find . \
			-path './examples/basic/build' -prune -o \
			-path './examples/clear-config/build' -prune -o \
			-path './test/build' -prune -o \
			-path './test/external' -prune -o \
			\( -iname '*.hpp' -o -iname '*.cpp' -o -iname '*.h' -o -iname '*.c' \) -print | \
			xargs clang-format --style=Chromium -i \
		"
.PHONY: format

check-format: image
	@docker run --rm -it \
		-v $(PWD)/CMakeLists.txt:/esp-idf-wifi-manager/CMakeLists.txt \
		-v $(PWD)/assets:/esp-idf-wifi-manager/assets \
		-v $(PWD)/examples:/esp-idf-wifi-manager/examples \
		-v $(PWD)/include:/esp-idf-wifi-manager/include \
		-v $(PWD)/src:/esp-idf-wifi-manager/src \
		-v $(PWD)/test:/esp-idf-wifi-manager/test \
		-w /esp-idf-wifi-manager/ \
		esp_idf_wifi_manager_image \
		sh -c "find . \
			-path './examples/basic/build' -prune -o \
			-path './examples/clear-config/build' -prune -o \
			-path './test/build' -prune -o \
			-path './test/external' -prune -o \
			\( -iname '*.hpp' -o -iname '*.cpp' -o -iname '*.h' -o -iname '*.c' \) -print | \
			xargs clang-format --style=Chromium --dry-run -Werror \
		"
.PHONY: check-format

test: image
	@docker run -it --rm \
		-v $(PWD)/CMakeLists.txt:/esp-idf-wifi-manager/CMakeLists.txt \
		-v $(PWD)/assets:/esp-idf-wifi-manager/assets \
		-v $(PWD)/examples:/esp-idf-wifi-manager/examples \
		-v $(PWD)/include:/esp-idf-wifi-manager/include \
		-v $(PWD)/src:/esp-idf-wifi-manager/src \
		-v $(PWD)/test:/esp-idf-wifi-manager/test \
		-w /esp-idf-wifi-manager/test/build \
		esp_idf_wifi_manager_image \
		sh -c " \
			cmake .. && \
			make && \
			./test \
		"
.PHONY: test

verify: build-basic check-format test
.PHONY: verify
