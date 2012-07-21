package com.dynamo.bob.pipeline;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.PrintWriter;

import com.dynamo.bob.BuilderParams;
import com.dynamo.bob.CopyBuilder;
import com.dynamo.bob.IResource;
import com.dynamo.bob.Task;

@BuilderParams(name = "FragmentProgram", inExts = ".fp", outExt = ".fpc")
public class FragmentProgramBuilder extends CopyBuilder {

    @Override
    public void build(Task<Void> task) throws IOException {
        IResource in = task.getInputs().get(0);
        IResource out = task.getOutputs().get(0);

        ByteArrayOutputStream os = new ByteArrayOutputStream(16 * 1024);
        PrintWriter writer = new PrintWriter(os);
        writer.println("#ifdef GL_ES");
        writer.println("precision mediump float;");
        writer.println("#endif");
        writer.close();
        os.write(in.getContent());
        os.close();
        out.setContent(os.toByteArray());
    }
}
