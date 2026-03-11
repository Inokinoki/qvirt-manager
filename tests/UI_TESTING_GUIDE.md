# QVirt-Manager UI Behavior Testing Guide

This document describes the UI behavior testing approach for QVirtManager using Qt Test.

## Overview

The UI behavior tests verify the user interface components, their interactions, and the overall user experience. These tests focus on:

1. **Widget existence and properties** - Verifying UI components are created correctly
2. **User interactions** - Testing button clicks, menu selections, and keyboard shortcuts
3. **Signal-slot connections** - Ensuring events trigger appropriate actions
4. **State management** - Testing UI state changes based on data and events
5. **Wizard workflows** - Validating multi-step dialog flows

## Test Structure

### Test Files

| Test File | Purpose |
|-----------|---------|
| `test_managerwindow.cpp` | Main window UI components and behavior |
| `test_contextmenu.cpp` | Context menu creation and signals |
| `test_createvmwizard.cpp` | VM creation wizard workflow |
| `test_dialogbehaviors.cpp` | Common dialog behaviors (Connection, Preferences, Delete, etc.) |
| `test_keyboardshortcuts.cpp` | Keyboard shortcuts and system tray |

### Existing Test Files

| Test File | Purpose |
|-----------|---------|
| `test_migrations.cpp` | Migration dialog functionality |
| `test_wizards.cpp` | Storage pool and network creation wizards |
| `test_graphwidget.cpp` | Graph widget visualization |
| `test_consoleviewers.cpp` | Console viewer functionality |
| `test_systemtray.cpp` | System tray icon behavior |
| `test_progressdialog.cpp` | Progress dialog display |

## Test Categories

### 1. Widget Existence Tests

Verify that UI components are created and accessible:

```cpp
void testButtonExists() {
    QPushButton *btn = m_window->findChild<QPushButton*>("buttonName");
    QVERIFY(btn != nullptr);
}
```

### 2. Property Tests

Check widget properties and initial states:

```cpp
void testWindowTitle() {
    QString title = m_window->windowTitle();
    QVERIFY(!title.isEmpty());
    QVERIFY(title.contains("QVirt Manager"));
}
```

### 3. Signal-Slot Tests

Verify that signals are connected correctly:

```cpp
void testSignalEmission() {
    QSignalSpy spy(m_object, &Object::signalName);
    QVERIFY(spy.isValid());

    emit m_object->signalName();
    QCOMPARE(spy.count(), 1);
}
```

### 4. Interaction Tests

Test user interactions with widgets:

```cpp
void testButtonClick() {
    QPushButton *btn = m_window->findChild<QPushButton*>("buttonName");
    QTest::mouseClick(btn, Qt::LeftButton);
    // Verify expected behavior
}
```

### 5. Workflow Tests

Validate multi-step processes (wizards, dialogs):

```cpp
void testWizardWorkflow() {
    m_wizard->restart();
    QCOMPARE(m_wizard->currentId(), Wizard::Page_Start);

    m_wizard->next();
    QCOMPARE(m_wizard->currentId(), Wizard::Page_Next);
}
```

## Running the Tests

### Build Tests

```bash
# From build directory
cmake ..
cmake --build .
```

### Run All Tests

```bash
ctest --output-on-failure
```

### Run Specific Test

```bash
# Run manager window tests
./tests/unit/test_managerwindow

# Run context menu tests
./tests/unit/test_contextmenu

# Run with verbose output
./tests/unit/test_managerwindow -v2
```

### Run Tests with GUI

```bash
# Some tests may display windows
export QT_QPA_PLATFORM=xcb
./tests/unit/test_managerwindow
```

## Test Coverage

### ManagerWindow Tests

- ✅ Window creation and initialization
- ✅ UI component existence (splitter, lists, buttons, toolbar, menubar, statusbar)
- ✅ Connection list configuration (model, selection mode, behavior)
- ✅ VM list configuration (model, selection mode, alternating colors, context menu)
- ✅ Control buttons (start, stop, reboot, pause, resume, new VM, delete, console)
- ✅ Action definitions
- ✅ Connection management

### ContextMenu Tests

- ✅ Context menu creation
- ✅ Menu action existence (start, stop, reboot, pause, resume, delete, clone, migrate, console, details, snapshot)
- ✅ Signal emission for all actions
- ✅ Action text validity
- ✅ Menu structure validation

### CreateVMWizard Tests

- ✅ Wizard creation and title
- ✅ Page existence (6 pages total)
- ✅ Page-specific field existence
- ✅ Navigation between pages
- ✅ Data getter methods
- ✅ Field validation
- ✅ Button existence (back, next, finish, cancel)

### Dialog Behavior Tests

- ✅ ConnectionDialog creation and fields
- ✅ PreferencesDialog creation and tabs
- ✅ DeleteDialog confirmation message
- ✅ CloneDialog name field
- ✅ HostDialog info fields
- ✅ Common dialog properties (OK/Cancel buttons, resizability)

### Keyboard Shortcuts Tests

- ✅ Shortcut constant definitions
- ✅ Shortcut action creation
- ✅ System tray functionality

## Testing Philosophy

### What We Test

1. **UI Structure** - Widgets exist and have correct properties
2. **User Interactions** - Buttons, menus, and keyboard shortcuts work
3. **Data Flow** - Data entered in wizards/dialogs is accessible
4. **Signal Connections** - Events trigger appropriate responses
5. **Workflows** - Multi-step processes complete correctly

### What We Don't Test

1. **Visual appearance** - Use visual regression testing for this
2. **Performance** - Use benchmarking tools for performance tests
3. **Accessibility** - Requires specialized accessibility testing tools
4. **Integration** - Integration tests are separate from unit tests

### Mocking Strategy

For UI tests:
- Use `nullptr` for Connection/Domain when possible
- Test UI structure without requiring libvirt backend
- Integration tests use real libvirt instances

## Adding New Tests

### 1. Create Test File

```cpp
#include <QtTest>
#include <QApplication>
#include "../../src/ui/YourWidget.h"

class TestYourWidget : public QObject
{
    Q_OBJECT

private slots:
    void testWidgetCreation();
    // Add more tests...
};
```

### 2. Update CMakeLists.txt

```cmake
add_executable(test_yourwidget test_yourwidget.cpp)
target_link_libraries(test_yourwidget
    qvirt-ui
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
)
add_test(NAME test_yourwidget COMMAND test_yourwidget)
```

### 3. Build and Run

```bash
cmake --build .
./tests/unit/test_yourwidget
```

## Best Practices

### 1. Test Isolation

Each test should be independent:
- Use `init()` and `cleanup()` for per-test setup
- Don't share state between tests
- Clean up resources in `cleanup()`

### 2. Descriptive Test Names

```cpp
// Good
void testConnectionListSelectionModeIsSingle();

// Avoid
void testList();
```

### 3. Assert Early

```cpp
// Good
QVERIFY(widget != nullptr);
QCOMPARE(widget->text(), QString("Expected"));

// Avoid
if (widget == nullptr) {
    return;
}
```

### 4. Use QSignalSpy

```cpp
QSignalSpy spy(object, &Object::signal);
QTRY_COMPARE(spy.count(), 1); // Wait for signal
```

### 5. Handle GUI Dependencies

```cpp
void initTestCase() {
    // Ensure QApplication exists
    if (!QApplication::instance()) {
        new QApplication(m_argc, m_argv);
    }
}
```

## Continuous Integration

These tests run in CI/CD pipelines:
- GitHub Actions
- GitLab CI
- Jenkins

### Example CI Configuration

```yaml
test:
  script:
    - cmake -B build
    - cmake --build build
    - cd build
    - ctest --output-on-failure
```

## Troubleshooting

### Headless Environment Issues

If tests fail in headless environments:
```bash
export QT_QPA_PLATFORM=offscreen
ctest
```

### Libvirt Connection Issues

Tests that don't require real connections:
- Use `nullptr` for Connection/Domain parameters
- Skip integration tests that need backend

### Widget Not Found

If `findChild<QWidget*>()` returns nullptr:
- Check widget object names
- Verify widget is created in `setupUI()`
- Use `findChildren<QWidget*>()` to list all widgets

### Signal Not Connected

If `QSignalSpy` reports signal not connected:
- Verify signal is declared with `signals:` keyword
- Check `connect()` statements in implementation
- Use `QObject::connect()` return value to verify connection

## Future Enhancements

### Planned Test Additions

1. **VMWindow Tests** - VM console and details window
2. **Snapshot Page Tests** - Snapshot management UI
3. **Storage Pool Dialog Tests** - Storage pool configuration
4. **Network Dialog Tests** - Network configuration
5. **Integration Tests** - End-to-end workflows with libvirt

### Testing Framework Improvements

1. **Visual Regression** - Screenshot comparison tests
2. **Performance Tests** - UI responsiveness benchmarks
3. **Accessibility Tests** - Screen reader compatibility
4. **Cross-Platform Tests** - Windows, macOS, Linux specific tests

## Resources

- [Qt Test Documentation](https://doc.qt.io/qt-6/qtest.html)
- [Qt Test Tutorial](https://doc.qt.io/qt-6/qtest-tutorial.html)
- [QVirt-Manager Developer Guide](../doc/developer-guide.md)
- [Libvirt API Reference](https://libvirt.org/html/

## Contributors

When adding new tests:
1. Follow existing test patterns
2. Add test descriptions to this document
3. Update CMakeLists.txt
4. Run tests before committing
5. Document any special test requirements

---

Last Updated: 2026-02-13
Maintainer: QVirt-Manager Team
