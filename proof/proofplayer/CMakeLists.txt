############################################################################
# CMakeLists.txt file for building ROOT proof/proofplayer package
# @author Pere Mato, CERN
############################################################################

ROOT_USE_PACKAGE(proof/proof)
ROOT_USE_PACKAGE(tree/treeplayer)

file(GLOB headers inc/*.h)
list(REMOVE_ITEM headers ${CMAKE_CURRENT_SOURCE_DIR}/inc/TProofDraw.h
                         ${CMAKE_CURRENT_SOURCE_DIR}/inc/LinkDef.h
                         ${CMAKE_CURRENT_SOURCE_DIR}/inc/LinkDefDraw.h)

file(GLOB sources src/*.cxx)
list(REMOVE_ITEM sources ${CMAKE_CURRENT_SOURCE_DIR}/src/TProofDraw.cxx)


ROOT_GENERATE_DICTIONARY(G__ProofPlayer ${headers} LINKDEF LinkDef.h)
ROOT_GENERATE_ROOTMAP(ProofPlayer LINKDEF LinkDef.h DEPENDENCIES  Proof Hist RIO Tree Net Thread MathCore)
ROOT_LINKER_LIBRARY(ProofPlayer ${sources} G__ProofPlayer.cxx LIBRARIES Proof DEPENDENCIES Hist)

ROOT_GENERATE_DICTIONARY(G__ProofDraw TProofDraw.h LINKDEF LinkDefDraw.h)
ROOT_GENERATE_ROOTMAP(ProofDraw LINKDEF LinkDefDraw.h DEPENDENCIES  TreePlayer Graf3d Graf Gpad ProofPlayer Hist Tree Proof)
ROOT_LINKER_LIBRARY(ProofDraw TProofDraw.cxx G__ProofDraw.cxx LIBRARIES ProofPlayer TreePlayer )

ROOT_INSTALL_HEADERS()



