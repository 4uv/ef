local function claimPet(identifier)
    local args = {
        [1] = identifier
    }
    game:GetService("ReplicatedStorage").Packages._Index["sleitnick_knit@1.4.7"].knit.Services.PetVoidService.RE.onClaim:FireServer(unpack(args))
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

local allPetIdentifiers = getAllPetIdentifiers()
for _, identifier in ipairs(allPetIdentifiers) do
    claimPet(identifier)
    wait() 
end
