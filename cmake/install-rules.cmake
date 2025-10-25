install(
    TARGETS ascimg_exe
    RUNTIME COMPONENT ascimg_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
