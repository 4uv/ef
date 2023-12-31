local sessionStats = {
    wins = 0,
    biceps = 0,
    hands = 0,
    knuckles = 0,
    totalEventEggs = 0
}
local webhookURL = getgenv().WEBHOOK_URL
local initialStats = {}  -- Initialize initialStats table

-- Function to calculate the difference in statistics since the last webhook
local function calculateDifference(statName, currentValue, initialValue)
    local difference = currentValue - initialValue
    return difference
end

-- Function to send the webhook message
local function sendWebhookMessage(embedFields)
    local localPlayer = game.Players.LocalPlayer
    local username = localPlayer.Name

    local data = {
        username = "RWS",
        avatar_url = "https://cdn.discordapp.com/attachments/949088173928103956/1128194972756234250/Png.png",
        embeds = {
            {
                title = username .. " stats",
                type = "rich",
                color = math.random(0x000000, 0xFFFFFF),
                fields = embedFields,
                footer = {
                    text = os.date("%I:%M:%S %p")
                }
            }
        }
    }

    local newdata = game:GetService("HttpService"):JSONEncode(data)

    local headers = {
        ["content-type"] = "application/json"
    }

    local request = http_request or request or HttpPost or syn.request
    local abcdef = {Url = webhookURL, Body = newdata, Method = "POST", Headers = headers}
    request(abcdef)
end

-- Function to retrieve the current statistics
local function getCurrentStatistics()
    local localPlayer = game.Players.LocalPlayer
    local leaderstats = localPlayer:FindFirstChild("leaderstats")
    local currentStats = {} -- Initialize currentStats table
    if leaderstats then
        local winsStat = leaderstats:FindFirstChild("Wins")
        local bicepsStat = leaderstats:FindFirstChild("Biceps")
        local handsStat = leaderstats:FindFirstChild("Hands")
        local knucklesStat = leaderstats:FindFirstChild("Knuckles")
        if winsStat then
            currentStats.wins = winsStat.Value
        end
        if bicepsStat then
            currentStats.biceps = bicepsStat.Value
        end
        if handsStat then
            currentStats.hands = handsStat.Value
        end
        if knucklesStat then
            currentStats.knuckles = knucklesStat.Value
        end
    end

    if localPlayer.PlayerGui.GameUI.Menus.Event.Amount then
        local amountText = localPlayer.PlayerGui.GameUI.Menus.Event.Amount.Text
        local amount = tonumber(string.match(amountText, "%d+"))
        if amount then
            currentStats.totalEventEggs = amount
        end
    end

    -- Get total strength
    local statText = localPlayer.Character.Head.HeadStat.Frame.Stat.Text
    local strength = (string.gsub(statText, ",", ""))
    if strength then
        currentStats.strength = strength
    end
    
    return currentStats
end

local function formatNumberWithCommas(number)
    local formatted = tostring(number)
    local parts = {}
    
    local decimalIndex = string.find(formatted, "%.")
    if decimalIndex then
        local integerPart = formatted:sub(1, decimalIndex - 1)
        local decimalPart = formatted:sub(decimalIndex)
        integerPart = string.reverse(integerPart)
        integerPart = string.gsub(integerPart, "(%d%d%d)", "%1,")
        integerPart = string.reverse(integerPart)
        if string.sub(integerPart, 1, 1) == "," then
            integerPart = string.sub(integerPart, 2)
        end
        formatted = integerPart .. decimalPart
    else
        formatted = string.reverse(formatted)
        formatted = string.gsub(formatted, "(%d%d%d)", "%1,")
        formatted = string.reverse(formatted)
        if string.sub(formatted, 1, 1) == "," then
            formatted = string.sub(formatted, 2)
        end
    end
    
    return formatted
end

initialStats = getCurrentStatistics()
-- Main loop
while true do
    wait(300) -- Wait for 5 Minutes
    
        -- Retrieve current statistics
        local currentStats = getCurrentStatistics()
    
        -- Calculate differences in statistics
        local embedFields = {}
        table.insert(embedFields, { name = "Total Strength", value = formatNumberWithCommas(currentStats.strength), inline = true })
        table.insert(embedFields, { name = "Total Event Eggs", value = formatNumberWithCommas(currentStats.totalEventEggs) .. " / " .. formatNumberWithCommas(sessionStats.totalEventEggs), inline = true })
        table.insert(embedFields, { name = "Total Wins", value = formatNumberWithCommas(currentStats.wins) .. " / " .. formatNumberWithCommas(sessionStats.wins), inline = true })
        table.insert(embedFields, { name = "Total Biceps", value = formatNumberWithCommas(currentStats.biceps) .. " / " .. formatNumberWithCommas(sessionStats.biceps), inline = true })
        table.insert(embedFields, { name = "Total Hands", value = formatNumberWithCommas(currentStats.hands) .. " / " .. formatNumberWithCommas(sessionStats.hands), inline = true })
        table.insert(embedFields, { name = "Total Knuckles", value = formatNumberWithCommas(currentStats.knuckles) .. " / " .. formatNumberWithCommas(sessionStats.knuckles), inline = true })
        table.insert(embedFields, { name = "Last 5 Minutes Event Eggs Earned", value = "+" .. formatNumberWithCommas(calculateDifference("Event Eggs", currentStats.totalEventEggs, initialStats.totalEventEggs)), inline = true })
        table.insert(embedFields, { name = "Last 5 Minutes Wins Earned", value = "+" .. formatNumberWithCommas(calculateDifference("Wins", currentStats.wins, initialStats.wins)), inline = true })
        table.insert(embedFields, { name = "Last 5 Minutes Biceps Earned", value = "+" .. formatNumberWithCommas(calculateDifference("Biceps", currentStats.biceps, initialStats.biceps)), inline = true })
        table.insert(embedFields, { name = "Last 5 Minutes Hands Earned", value = "+" .. formatNumberWithCommas(calculateDifference("Hands", currentStats.hands, initialStats.hands)), inline = true })
        table.insert(embedFields, { name = "Last 5 Minutes Knuckles Earned", value = "+" .. formatNumberWithCommas(calculateDifference("Knuckles", currentStats.knuckles, initialStats.knuckles)), inline = true })

        -- Send the webhook message
        sendWebhookMessage(embedFields)

        -- Update session statistics for the next iteration
        sessionStats.wins = sessionStats.wins + calculateDifference("Wins", currentStats.wins, initialStats.wins)
        sessionStats.biceps = sessionStats.biceps + calculateDifference("Biceps", currentStats.biceps, initialStats.biceps)
        sessionStats.hands = sessionStats.hands + calculateDifference("Hands", currentStats.hands, initialStats.hands)
        sessionStats.knuckles = sessionStats.knuckles + calculateDifference("Knuckles", currentStats.knuckles, initialStats.knuckles)
        sessionStats.totalEventEggs = sessionStats.totalEventEggs + calculateDifference("Event Eggs", currentStats.totalEventEggs, initialStats.totalEventEggs)

        -- Update initial statistics for the next iteration
        initialStats.wins = currentStats.wins
        initialStats.biceps = currentStats.biceps
        initialStats.hands = currentStats.hands
        initialStats.knuckles = currentStats.knuckles
        initialStats.totalEventEggs = currentStats.totalEventEggs
    end
