package com.dynamo.bob.pipeline;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import com.dynamo.bob.Builder;
import com.dynamo.bob.CompileExceptionError;
import com.dynamo.bob.test.util.PropertiesTestUtil;
import com.dynamo.bob.util.MurmurHash;
import com.dynamo.lua.proto.Lua.LuaModule;
import com.dynamo.properties.proto.PropertiesProto.PropertyDeclarations;
import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.Message;

public class LuaBuilderTest extends AbstractProtoBuilderTest {

    @Override
    protected Builder<Void> createBuilder() {
        return new ScriptBuilders.ScriptBuilder();
    }

    @Override
    protected Message parseMessage(byte[] content) throws InvalidProtocolBufferException {
        return LuaModule.parseFrom(content);
    }

    @Test
    public void testProps() throws Exception {
        StringBuilder src = new StringBuilder();
        src.append("\n");
        src.append("go.property(\"number\", 1)\n");
        src.append("go.property(\"hash\", hash(\"hash\"))\n");
        src.append("go.property(\"url\", msg.url())\n");
        src.append("go.property(\"vec3\", vmath.vector3(1, 2, 3))\n");
        src.append("go.property(\"vec4\", vmath.vector4(4, 5, 6, 7))\n");
        src.append("go.property(\"quat\", vmath.quat(8, 9, 10, 11))\n");
        src.append("\n");
        src.append("    go.property(  \"space_number\"  ,  1   )\n");
        LuaModule luaModule = (LuaModule)build("/test.script", src.toString());
        PropertyDeclarations properties = luaModule.getProperties();
        assertEquals(2, properties.getNumberEntriesCount());
        PropertiesTestUtil.assertNumber(properties, 1, 0);
        PropertiesTestUtil.assertNumber(properties, 1, 1);
        PropertiesTestUtil.assertHash(properties, MurmurHash.hash64("hash"), 0);
        PropertiesTestUtil.assertURL(properties, "", 0);
        PropertiesTestUtil.assertVector3(properties, 1, 2, 3, 0);
        PropertiesTestUtil.assertVector4(properties, 4, 5, 6, 7, 0);
        PropertiesTestUtil.assertQuat(properties, 8, 9, 10, 11, 0);
    }

    @Test
    public void testPropUnsupportedType() throws Exception {
        StringBuilder src = new StringBuilder();
        src.append("\n");
        src.append("go.property(\"string\", \"\")\n");
        try {
            @SuppressWarnings("unused")
            LuaModule luaModule = (LuaModule)build("/test.script", src.toString());
            assertTrue(false);
        } catch (CompileExceptionError e) {
            assertEquals(2, e.getLineNumber());
        }
    }
}
