local function craftPetToNextTier(identifier)
    local args = {
        [1] = identifier,
        [2] = false,
        [3] = true
    }
    game:GetService("ReplicatedStorage").Packages._Index["sleitnick_knit@1.4.7"].knit.Services.PetService.RF.craft:InvokeServer(unpack(args))
end

local localPlayer = game:GetService("Players").LocalPlayer

local petInventory = localPlayer.PlayerGui.GameUI.Menus.PetInventory.Container.ScrollingFrame.Pets

local function getAllPetIdentifiers()
    local identifiers = {}
    for _, pet in pairs(petInventory:GetChildren()) do
        if pet:IsA("Frame") then
            local petId = pet.Name
            table.insert(identifiers, petId)
        end
    end
    return identifiers
end

local function shouldSkipPet(identifier)
    local pet = petInventory:FindFirstChild(identifier)
    if pet and pet:IsA("Frame") then
        local attributes = pet.Toggle:GetAttributes()
        return attributes.CraftType == "Huge" or attributes.EndTime or attributes.VoidMachine
    end
    return false
end

local allPetIdentifiers = getAllPetIdentifiers()
local petsAttempted = 0
for _, identifier in ipairs(allPetIdentifiers) do
    if not shouldSkipPet(identifier) then
        craftPetToNextTier(identifier)
        wait() 
        petsAttempted = petsAttempted + 1
    end
end

if petsAttempted == 0 then
    Rayfield:Notify({
        Title = "Notification",
        Content = "All pets have been checked.",
        Duration = 6.5,
        Image = 4483362458,
    })
else
    Rayfield:Notify({
        Title = "Notification",
        Content = "All pets have been checked!",
        Duration = 6.5,
        Image = 4483362458,
    })
end
