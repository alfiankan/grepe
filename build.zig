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

    const grepe_test_ui_bin = std.build.ExecutableOptions{
        .name = "grepe-tui-test",
        .root_source_file = .{ .path = "src/tui/realtime_bar_test.c" },
        .target = target,
        .optimize = mode,
    };

    const grepe_test_tui = b.addExecutable(grepe_test_ui_bin);
    grepe_test_tui.addCSourceFiles(&.{"src/common.c"}, &.{ "-isystem", "-std=c11", "-Wall", "-pedantic", "-lncurses" });
    grepe_test_tui.linkLibC();
    b.installArtifact(grepe_test_tui);
}
