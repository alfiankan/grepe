const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    const target = b.standardTargetOptions(.{});
    const mode = b.standardOptimizeOption(.{});

    const grepe_bin = std.build.ExecutableOptions{
        .name = "grepe",
        .root_source_file = .{ .path = "src/cmd.c" },
        .target = target,
        .optimize = mode,
    };

    const grepe_main = b.addExecutable(grepe_bin);
    grepe_main.addCSourceFiles(&.{"src/common.c"}, &.{"-std=c90"});
    grepe_main.linkLibC();
    b.installArtifact(grepe_main);

    const grepe_test_bin = std.build.ExecutableOptions{
        .name = "grepe-test",
        .root_source_file = .{ .path = "src/tests/test.c" },
        .target = target,
        .optimize = mode,
    };

    const grepe_test = b.addExecutable(grepe_test_bin);
    grepe_test.addCSourceFiles(&.{"src/common.c"}, &.{"-std=c90"});
    grepe_test.linkLibC();
    b.installArtifact(grepe_test);
}
