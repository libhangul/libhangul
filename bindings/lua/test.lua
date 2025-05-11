local hangul = require 'hangul'
local hangul_ic = hangul.ic_new()
local text = ''
while true do
    local char = string.byte(io.read(1))
    if char ~= '\n' then
        hangul_ic:process(char)
    end
    local commitString = hangul.ucs4_to_utf8(hangul_ic:get_commit_string())
    if commitString ~= '' then
        text = text .. commitString
        print(text)
    end
end
