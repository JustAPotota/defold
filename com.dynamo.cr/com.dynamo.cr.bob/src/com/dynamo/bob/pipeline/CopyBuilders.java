package com.dynamo.bob.pipeline;

import com.dynamo.bob.BuilderParams;
import com.dynamo.bob.CopyBuilder;

public class CopyBuilders {
    @BuilderParams(name = "Project", inExts = ".project", outExt = ".projectc")
    public static class ProjectBuilder extends CopyBuilder {}

    @BuilderParams(name = "Script", inExts = ".script", outExt = ".scriptc")
    public static class ScriptBuilder extends CopyBuilder {}

    @BuilderParams(name = "GuiScript", inExts = ".gui_script", outExt = ".gui_scriptc")
    public static class GuiScriptBuilder extends CopyBuilder {}

    @BuilderParams(name = "RenderScript", inExts = ".render_script", outExt = ".render_scriptc")
    public static class RenderScriptBuilder extends CopyBuilder {}

    @BuilderParams(name = "Wav", inExts = ".wav", outExt = ".wavc")
    public static class WavBuilder extends CopyBuilder {}

}
