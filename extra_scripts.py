Import("env", "projenv")

env.Replace(    
    UPLOADCMD="curl --request POST --data-binary @\"$BUILD_DIR\\${PROGNAME}.bin\" $UPLOADERFLAGS"
)

if False:
    env.AddPostAction(
        "$BUILD_DIR/${PROGNAME}.elf",
        env.VerboseAction(" ".join([
            "$OBJCOPY", "-O", "binary",
            "\"$BUILD_DIR\\${PROGNAME}.elf", "\"$BUILD_DIR\\${PROGNAME}.bin"
        ]), "Building $BUILD_DIR/${PROGNAME}.bin")
    )