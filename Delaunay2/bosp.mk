
ifdef CONFIG_SAMPLES_OPENCV_DELAUNAY2

# Targets provided by this project
.PHONY: samples_opencv_delaunay2 clean_samples_opencv_delaunay2

# Add this to the "samples" targets
samples_opencv: samples_opencv_delaunay2
clean_samples_opencv: clean_samples_opencv_delaunay2

MODULE_SAMPLES_OPENCV_DELAUNAY2=samples/opencv/Delaunay2

samples_opencv_delaunay2: external
	@echo
	@echo "==== Building Delaunay2 ($(BUILD_TYPE)) ===="
	@echo " Using GCC    : $(CC)"
	@echo " Target flags : $(TARGET_FLAGS)"
	@echo " Sysroot      : $(PLATFORM_SYSROOT)"
	@echo " BOSP Options : $(CMAKE_COMMON_OPTIONS)"
	@[ -d $(MODULE_SAMPLES_OPENCV_DELAUNAY2)/build/$(BUILD_TYPE) ] || \
		mkdir -p $(MODULE_SAMPLES_OPENCV_DELAUNAY2)/build/$(BUILD_TYPE) || \
		exit 1
	@cd $(MODULE_SAMPLES_OPENCV_DELAUNAY2)/build/$(BUILD_TYPE) && \
		CC=$(CC) CFLAGS="$(TARGET_FLAGS)" \
		CXX=$(CXX) CXXFLAGS="$(TARGET_FLAGS)" \
		cmake $(CMAKE_COMMON_OPTIONS) ../.. || \
		exit 1
	@cd $(MODULE_SAMPLES_OPENCV_DELAUNAY2)/build/$(BUILD_TYPE) && \
		make -j$(CPUS) install || \
		exit 1

clean_samples_opencv_delaunay2:
	@echo
	@echo "==== Clean-up Delaunay2 Application ===="
	@[ ! -f $(BUILD_DIR)/usr/bin/delaunay2 ] || \
		rm -f $(BUILD_DIR)/etc/bbque/recipes/Delaunay2*; \
		rm -f $(BUILD_DIR)/usr/bin/delaunay2*
	@rm -rf $(MODULE_SAMPLES_OPENCV_DELAUNAY2)/build
	@echo

else # CONFIG_SAMPLES_OPENCV_DELAUNAY2

samples_opencv_delaunay2:
	$(warning Delaunay2 module disabled by BOSP configuration)
	$(error BOSP compilation failed)

endif # CONFIG_SAMPLES_OPENCV_DELAUNAY2

