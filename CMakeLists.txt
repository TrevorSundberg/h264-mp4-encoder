cmake_minimum_required(VERSION 3.13)
project(encoder)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fpermissive")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fpermissive")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

set(SOURCE_FILES
    main.cpp
    h264_mp4_encoder.cpp

    libmp4v2/src/3gp.cpp
    libmp4v2/src/atom_ac3.cpp
    libmp4v2/src/atom_amr.cpp
    libmp4v2/src/atom_avc1.cpp
    libmp4v2/src/atom_avcC.cpp
    libmp4v2/src/atom_chpl.cpp
    libmp4v2/src/atom_colr.cpp
    libmp4v2/src/atom_d263.cpp
    libmp4v2/src/atom_dac3.cpp
    libmp4v2/src/atom_damr.cpp
    libmp4v2/src/atom_dref.cpp
    libmp4v2/src/atom_elst.cpp
    libmp4v2/src/atom_enca.cpp
    libmp4v2/src/atom_encv.cpp
    libmp4v2/src/atom_free.cpp
    libmp4v2/src/atom_ftab.cpp
    libmp4v2/src/atom_ftyp.cpp
    libmp4v2/src/atom_gmin.cpp
    libmp4v2/src/atom_hdlr.cpp
    libmp4v2/src/atom_hinf.cpp
    libmp4v2/src/atom_hnti.cpp
    libmp4v2/src/atom_href.cpp
    libmp4v2/src/atom_mdat.cpp
    libmp4v2/src/atom_mdhd.cpp
    libmp4v2/src/atom_meta.cpp
    libmp4v2/src/atom_mp4s.cpp
    libmp4v2/src/atom_mp4v.cpp
    libmp4v2/src/atom_mvhd.cpp
    libmp4v2/src/atom_nmhd.cpp
    libmp4v2/src/atom_ohdr.cpp
    libmp4v2/src/atom_pasp.cpp
    libmp4v2/src/atom_root.cpp
    libmp4v2/src/atom_rtp.cpp
    libmp4v2/src/atom_s263.cpp
    libmp4v2/src/atom_sdp.cpp
    libmp4v2/src/atom_sdtp.cpp
    libmp4v2/src/atom_smi.cpp
    libmp4v2/src/atom_sound.cpp
    libmp4v2/src/atom_standard.cpp
    libmp4v2/src/atom_stbl.cpp
    libmp4v2/src/atom_stdp.cpp
    libmp4v2/src/atom_stsc.cpp
    libmp4v2/src/atom_stsd.cpp
    libmp4v2/src/atom_stsz.cpp
    libmp4v2/src/atom_stz2.cpp
    libmp4v2/src/atom_text.cpp
    libmp4v2/src/atom_tfhd.cpp
    libmp4v2/src/atom_tkhd.cpp
    libmp4v2/src/atom_treftype.cpp
    libmp4v2/src/atom_trun.cpp
    libmp4v2/src/atom_tx3g.cpp
    libmp4v2/src/atom_udta.cpp
    libmp4v2/src/atom_url.cpp
    libmp4v2/src/atom_urn.cpp
    libmp4v2/src/atom_uuid.cpp
    libmp4v2/src/atom_video.cpp
    libmp4v2/src/atom_vmhd.cpp
    libmp4v2/src/bmff/typebmff.cpp
    libmp4v2/src/cmeta.cpp
    libmp4v2/src/descriptors.cpp
    libmp4v2/src/enum.tcc
    libmp4v2/src/exception.cpp
    libmp4v2/src/isma.cpp
    libmp4v2/src/itmf/CoverArtBox.cpp
    libmp4v2/src/itmf/generic.cpp
    libmp4v2/src/itmf/Tags.cpp
    libmp4v2/src/itmf/type.cpp
    libmp4v2/src/log.cpp
    libmp4v2/src/mp4.cpp
    libmp4v2/src/mp4atom.cpp
    libmp4v2/src/mp4container.cpp
    libmp4v2/src/mp4descriptor.cpp
    libmp4v2/src/mp4file_io.cpp
    libmp4v2/src/mp4file.cpp
    libmp4v2/src/mp4info.cpp
    libmp4v2/src/mp4property.cpp
    libmp4v2/src/mp4track.cpp
    libmp4v2/src/mp4util.cpp
    libmp4v2/src/ocidescriptors.cpp
    libmp4v2/src/odcommands.cpp
    libmp4v2/src/qosqualifiers.cpp
    libmp4v2/src/qtff/coding.cpp
    libmp4v2/src/qtff/ColorParameterBox.cpp
    libmp4v2/src/qtff/PictureAspectRatioBox.cpp
    libmp4v2/src/rtphint.cpp
    libmp4v2/src/text.cpp

    libmp4v2/libplatform/io/File_posix.cpp
    libmp4v2/libplatform/io/File.cpp
    libmp4v2/libplatform/io/FileSystem_posix.cpp
    libmp4v2/libplatform/io/FileSystem.cpp
    libmp4v2/libplatform/number/random_posix.cpp
    libmp4v2/libplatform/process/process_posix.cpp
    libmp4v2/libplatform/prog/option.cpp
    libmp4v2/libplatform/sys/error.cpp
    libmp4v2/libplatform/time/time_posix.cpp
    libmp4v2/libplatform/time/time.cpp
)

add_executable(encoder ${SOURCE_FILES})

target_include_directories(encoder PRIVATE
    "generated"
    "libmp4v2/include"
    "libmp4v2"
    "minih264"
)