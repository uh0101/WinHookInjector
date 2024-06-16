# WinHookInjector

WinHookInjector is a tool designed for injecting DLLs into processes for the purpose of hooking and modifying behavior.

## Compatibility

This injector is compatible with DLLs that follow a simple structure, such as:

```cpp
#include <Windows.h>

extern "C" __declspec(dllexport)
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
    switch (dwReason) {
    case DLL_PROCESS_ATTACH:
        MessageBoxA(NULL, "Injected successfully.", "WinHookInjector", MB_ICONINFORMATION | MB_OK);
        break;
    default: break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) LRESULT NextHook(int code, WPARAM wParam, LPARAM lParam) {
    return CallNextHookEx(NULL, code, wParam, lParam);
}
```

This example DLL demonstrates the required structure:
- **NextHook function**: Exported function that can be used to set up hooks in applications.

## Usage

To use WinHookInjector, follow these steps:

1. **Download the Injector**: Get the latest build from [GitHub Actions Artifact](https://github.com/uh0101/WinHookInjector/actions) (You can also get it from releases but it's probably outdated).

2. **Inject DLL into a Process**:
   ```
   WinHookInjector <DLL> <Process>
   ```
   - `<DLL>`: Specify the path to the DLL you wish to inject.
   - `<Process>`: Provide the name of the target process in which the DLL will be injected.

### Example

For example, to inject `Example.dll` into `notepad.exe`:

```
WinHookInjector Example.dll notepad.exe
```

## Security Considerations

DLL injection can pose risks if used improperly:

- Ensure you have appropriate permissions to modify the target process.
- Use WinHookInjector responsibly and comply with legal and ethical guidelines.
- Consider potential stability issues and mitigate risks through testing in controlled environments.

## Contributing

Contributions to WinHookInjector are welcome! Here's how you can contribute:

- Submit bug reports or feature requests through [GitHub Issues](https://github.com/uh0101/WinHookInjector/issues).
- Fork the repository, make improvements, and submit [pull requests](https://github.com/uh0101/WinHookInjector/pulls) for review.
- Follow coding standards and ensure compatibility across different environments.

## License

WinHookInjector is licensed under the MIT License. See [LICENSE file](https://github.com/uh0101/WinHookInjector/blob/main/LICENSE) for details.

## Troubleshooting

If you encounter issues with WinHookInjector:

- **Issue**: WinHookInjector fails to inject DLL into a process.
  - **Solution**: Ensure both WinHookInjector and the DLL are compiled for the correct architecture.

## Future Roadmap

Future plans for WinHookInjector include:

- Implementing support for injecting multiple DLLs into a single process.
- Enhancing error handling and logging capabilities for better diagnostic information.
