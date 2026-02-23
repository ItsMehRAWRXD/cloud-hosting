; ============================================================================
; UAC Privilege Check (MASM x64)
; ============================================================================
; Checks whether the current process is running with elevated (admin)
; privileges.  This is a read-only check — it does NOT bypass or modify
; any security settings.
;
; Integrated into the RawrXD-Crypto target via CMakeLists.txt.
; ============================================================================

.code

; External Windows API functions
EXTERN OpenProcessToken:PROC
EXTERN GetTokenInformation:PROC
EXTERN GetCurrentProcess:PROC
EXTERN CloseHandle:PROC

; ============================================================================
; Constants
; ============================================================================
TOKEN_QUERY            EQU 8h
TokenElevation         EQU 14h   ; TOKEN_INFORMATION_CLASS value (20 decimal)
TOKEN_ELEVATION_SIZE   EQU 4     ; sizeof(TOKEN_ELEVATION)

; ============================================================================
; IsProcessElevated — check if the current process has admin privileges
;
; Parameters:   (none)
;
; Returns:
;   RAX = 1  if elevated (admin)
;   RAX = 0  if not elevated (standard user)
;   RAX = -1 on error
; ============================================================================
IsProcessElevated PROC
    push rbp
    mov rbp, rsp
    sub rsp, 40h          ; shadow space + locals
    push rbx

    ; --- locals on the stack ---
    ; [rbp-8]   hToken       (QWORD)
    ; [rbp-12]  elevation    (DWORD — TOKEN_ELEVATION.TokenIsElevated)
    ; [rbp-16]  returnLength (DWORD)
    mov QWORD PTR [rbp-8],  0
    mov DWORD PTR [rbp-12], 0
    mov DWORD PTR [rbp-16], 0

    ; 1) Get pseudo-handle for current process
    call GetCurrentProcess
    ; RAX = process pseudo-handle

    ; 2) Open the process token (TOKEN_QUERY)
    mov rcx, rax                       ; hProcess
    mov edx, TOKEN_QUERY               ; DesiredAccess
    lea r8,  [rbp-8]                   ; &hToken
    call OpenProcessToken
    test eax, eax
    jz  elevated_error

    ; 3) Query token for elevation info
    mov rcx, [rbp-8]                   ; hToken
    mov edx, TokenElevation            ; TokenInformationClass
    lea r8,  [rbp-12]                  ; &elevation (output)
    mov r9d, TOKEN_ELEVATION_SIZE      ; size
    lea rax, [rbp-16]
    mov [rsp+20h], rax                 ; &returnLength (5th arg on stack)
    call GetTokenInformation
    test eax, eax
    jz  elevated_close_error

    ; 4) Read result
    mov eax, DWORD PTR [rbp-12]        ; elevation.TokenIsElevated
    mov ebx, eax                       ; save in non-volatile register

    ; 5) Close the token handle
    mov rcx, [rbp-8]
    call CloseHandle

    ; Return 1 (elevated) or 0 (not elevated)
    movzx eax, bl
    jmp elevated_done

elevated_close_error:
    ; Close handle before returning error
    mov rcx, [rbp-8]
    call CloseHandle

elevated_error:
    mov eax, -1                        ; indicate error

elevated_done:
    pop rbx
    add rsp, 40h
    pop rbp
    ret
IsProcessElevated ENDP

END
